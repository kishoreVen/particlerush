using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LibGit2Sharp;

namespace AssetFolderGenerator
{
    class GitManager
    {
        #region Constants
        #endregion

        #region Private Variables
        private string                                      _commitMessage;
        private LibGit2Sharp.Repository                     _workingRepository;
        private LibGit2Sharp.CommitOptions                  _commitOptions;
        private LibGit2Sharp.StageOptions                   _stageOptions;
        private LibGit2Sharp.PullOptions                    _pullOptions;
        private LibGit2Sharp.PushOptions                    _pushOptions;
        private LibGit2Sharp.Remote                         _remote;
        private LibGit2Sharp.ExplicitPathsOptions           _explicitPathOptions;
        #endregion

        #region Public Variables
        #endregion

        #region Constructor
        public GitManager()
        {
            _workingRepository = null;

            _commitMessage = string.Empty;

            _commitOptions = new LibGit2Sharp.CommitOptions();
            _commitOptions.AmendPreviousCommit = false;
            _commitOptions.AllowEmptyCommit = false;
            _commitOptions.PrettifyMessage = false;
            
            _stageOptions = new LibGit2Sharp.StageOptions();

            _pullOptions = new LibGit2Sharp.PullOptions();
            _pullOptions.FetchOptions = new LibGit2Sharp.FetchOptions();
            _pullOptions.MergeOptions = new LibGit2Sharp.MergeOptions();

            _pushOptions = new LibGit2Sharp.PushOptions();

            _explicitPathOptions = new LibGit2Sharp.ExplicitPathsOptions();
            _explicitPathOptions.ShouldFailOnUnmatchedPath = false;
        }        
        #endregion

        #region Private Methods
        private LibGit2Sharp.Signature GetAuthorSignature(string username, string password)
        {
            return new LibGit2Sharp.Signature(username, password, DateTime.Now);
        }
        #endregion

        #region Public Methods
        public void AuthenticateRepository(string username, string password, string repositoryRoot)
        {
            try
            {
                _pullOptions.FetchOptions.CredentialsProvider = new LibGit2Sharp.Handlers.CredentialsHandler(
                    (_url, _user, _cred) => new LibGit2Sharp.UsernamePasswordCredentials
                    {
                        Username = username,
                        Password = password
                    }
                );

                _pushOptions.CredentialsProvider = new LibGit2Sharp.Handlers.CredentialsHandler(
                    (_url, _user, _cred) => new LibGit2Sharp.UsernamePasswordCredentials
                    {
                        Username = username,
                        Password = password
                    }
                );

                _workingRepository      = new LibGit2Sharp.Repository(repositoryRoot);

                _remote = _workingRepository.Network.Remotes["origin"];
            }
            catch(System.Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("You need to choose the root directory of your project's git repository to create a directory structure. If you have done it present the following information to developer. " + ex.Message, "Repository Root Not Selected",  System.Windows.Forms.MessageBoxButtons.OK,  System.Windows.Forms.MessageBoxIcon.Error);
            }            
        }

        public void SyncRepository(string username, string password)
        {
            try
            {
                _workingRepository.Network.Pull(GetAuthorSignature(username, password), _pullOptions);
            }
            catch (System.Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Sync Failure. Check Username and Network. If correct, present this info to developer: " + ex.Message, "Sync Failure",  System.Windows.Forms.MessageBoxButtons.OK,  System.Windows.Forms.MessageBoxIcon.Error);
            }
        }

        public void PushChangesToRepository(string username, string password)
        {
            if(_commitMessage == string.Empty)
                return;
            
            string commitMessage = "#Auto Commit -- @" + username + ":" + _commitMessage;

            try
            {
                _workingRepository.Commit(commitMessage, GetAuthorSignature(username, password), GetAuthorSignature(username, password), _commitOptions);
            }
            catch(System.Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Commit Failure. Check Username and Network. If correct, present this info to developer: " + ex.Message, "Commit Failure",  System.Windows.Forms.MessageBoxButtons.OK,  System.Windows.Forms.MessageBoxIcon.Error);
            }

            try
            {
                var pushRefSpec = @"refs/heads/master";
                _workingRepository.Network.Push(_remote, pushRefSpec, _pushOptions, GetAuthorSignature(username, password), "pushed latest changes");
            }
            catch (System.Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Push Failure. Check Username, Password and Network. If correct, present this info to developer: " + ex.Message, "Push Failure",  System.Windows.Forms.MessageBoxButtons.OK,  System.Windows.Forms.MessageBoxIcon.Error);
            }
            
            _commitMessage = string.Empty;
        }

        public void StageFileForCommit(string fullFileName, string additionalCommitMessage = "Updated/Added file.")
        {
            try
            {
                LibGit2Sharp.FileStatus status = _workingRepository.RetrieveStatus(fullFileName);

                if (status == LibGit2Sharp.FileStatus.Added || status == LibGit2Sharp.FileStatus.Modified || status == LibGit2Sharp.FileStatus.Removed)
                {
                    _workingRepository.Stage(fullFileName, _stageOptions);
                    _commitMessage += "\n + " + additionalCommitMessage;
                }
            }
            catch(System.Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Stage Failure: " + ex.Message, "Stage Failure",  System.Windows.Forms.MessageBoxButtons.OK,  System.Windows.Forms.MessageBoxIcon.Error);
            }            
        }

        public void StageDirectoryForCommit(string fullDirectoryPath, string additionalCommitMessage = "Updated/Added Directory")
        {
            try
            {
                _workingRepository.Stage(fullDirectoryPath, _stageOptions);
                _commitMessage += "\n + " + additionalCommitMessage;
            }
            catch(System.Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Stage Failure: " + ex.Message, "Stage Failure",  System.Windows.Forms.MessageBoxButtons.OK,  System.Windows.Forms.MessageBoxIcon.Error);
            }
        }

        public void UnstageFile(string fullFileName)
        {
            if(_workingRepository.RetrieveStatus(fullFileName) == LibGit2Sharp.FileStatus.Staged)
            {
                _workingRepository.Unstage(fullFileName, _explicitPathOptions);
            }            
        }

        public void UnstageDirectory(string fullDirectoryPath)
        {
            if (_workingRepository.RetrieveStatus(fullDirectoryPath) == LibGit2Sharp.FileStatus.Staged)
            {
                _workingRepository.Unstage(fullDirectoryPath, _explicitPathOptions);
            }
        }
        #endregion

        #region Private Methods
        #endregion
    }
}
