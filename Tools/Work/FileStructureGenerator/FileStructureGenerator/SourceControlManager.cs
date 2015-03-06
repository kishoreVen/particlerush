using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;
using System.Windows;
using Newtonsoft.Json;

namespace FileStructureGenerator
{
    public class SourceControlManager
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
        public SourceControlManager()
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
        private LibGit2Sharp.Signature GetAuthorSignature()
        {
            return new LibGit2Sharp.Signature(Globals.BitbucketUsername, Globals.BitbucketEmail, DateTime.Now);
        }
        #endregion

        #region Public Methods
        public void AuthenticateRepository()
        {
            try
            {
                _pullOptions.FetchOptions.CredentialsProvider = new LibGit2Sharp.Handlers.CredentialsHandler(
                    (_url, _user, _cred) => new LibGit2Sharp.UsernamePasswordCredentials
                    {
                        Username = Globals.BitbucketUsername,
                        Password = Globals.BitbucketPassword
                    }
                );

                _pushOptions.CredentialsProvider = new LibGit2Sharp.Handlers.CredentialsHandler(
                    (_url, _user, _cred) => new LibGit2Sharp.UsernamePasswordCredentials
                    {
                        Username = Globals.BitbucketUsername,
                        Password = Globals.BitbucketPassword
                    }
                );

                _workingRepository      = new LibGit2Sharp.Repository(Globals.RepositoryRoot);

                _remote = _workingRepository.Network.Remotes["origin"];
            }
            catch(System.Exception ex)
            {
                System.Windows.MessageBox.Show("You need to choose the root directory of your project's git repository to create a directory structure. If you have done it present the following information to developer. " + ex.Message, "Repository Root Not Selected", MessageBoxButton.OK, MessageBoxImage.Error);
            }            
        }

        public void SyncRepository()
        {
            try
            {
                _workingRepository.Network.Pull(GetAuthorSignature(), _pullOptions);
            }
            catch (System.Exception ex)
            {
                System.Windows.MessageBox.Show("Sync Failure. Check Username and Network. If correct, present this info to developer: " + ex.Message, "Sync Failure", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        public void PushChangesToRepository()
        {
            if(_commitMessage == string.Empty)
                return;
            
            string commitMessage = "#Auto Commit -- @" + Globals.BitbucketUsername + ":" + _commitMessage;

            try
            {
                _workingRepository.Commit(commitMessage, GetAuthorSignature(), GetAuthorSignature(), _commitOptions);                
            }
            catch(System.Exception ex)
            {
                System.Windows.MessageBox.Show("Commit Failure. Check Username and Network. If correct, present this info to developer: " + ex.Message, "Commit Failure", MessageBoxButton.OK, MessageBoxImage.Error);
            }

            try
            {
                var pushRefSpec = @"refs/heads/master";
                _workingRepository.Network.Push(_remote, pushRefSpec, _pushOptions, GetAuthorSignature(), "pushed latest changes");
            }
            catch (System.Exception ex)
            {
                System.Windows.MessageBox.Show("Push Failure. Check Username, Password and Network. If correct, present this info to developer: " + ex.Message, "Push Failure", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            
            _commitMessage = string.Empty;
        }

        public void StageFileForCommit(string fileNameRelativeToRepoRoot, string additionalCommitMessage = "Updated/Added file.")
        {
            string filePath = Globals.RepositoryRoot + fileNameRelativeToRepoRoot;

            try
            {
                LibGit2Sharp.FileStatus status = _workingRepository.RetrieveStatus(filePath);

                if (status == LibGit2Sharp.FileStatus.Added || status == LibGit2Sharp.FileStatus.Modified || status == LibGit2Sharp.FileStatus.Removed)
                {
                    _workingRepository.Stage(filePath, _stageOptions);

                    _commitMessage += "\n + " + fileNameRelativeToRepoRoot + ": " + additionalCommitMessage;
                }
            }
            catch(System.Exception ex)
            {
                System.Windows.MessageBox.Show("Stage Failure: " + ex.Message, "Stage Failure", MessageBoxButton.OK, MessageBoxImage.Error);
            }            
        }

        public void StageDirectoryForCommit(string directoryPathRelativeToRepoRoot, string additionalCommitMessage = "Updated/Added Directory")
        {
            string directoryPath = Globals.RepositoryRoot + directoryPathRelativeToRepoRoot;

            try
            {
                _workingRepository.Stage(directoryPath, _stageOptions);

                _commitMessage += "\n + " + directoryPathRelativeToRepoRoot + ": " + additionalCommitMessage;
            }
            catch(System.Exception ex)
            {
                System.Windows.MessageBox.Show("Stage Failure: " + ex.Message, "Stage Failure", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        public void UnstageFile(string fileNameRelativeToRepoRoot)
        {
            string filePath = Globals.RepositoryRoot + fileNameRelativeToRepoRoot;

            if(_workingRepository.RetrieveStatus(filePath) == LibGit2Sharp.FileStatus.Staged)
            {
                _workingRepository.Unstage(fileNameRelativeToRepoRoot, _explicitPathOptions);
            }            
        }

        public void UnstageDirectory(string directoryPathRelativeToRepoRoot)
        {
            string directoryPath = Globals.RepositoryRoot + directoryPathRelativeToRepoRoot;

            if (_workingRepository.RetrieveStatus(directoryPath) == LibGit2Sharp.FileStatus.Staged)
            {
                _workingRepository.Unstage(directoryPathRelativeToRepoRoot, _explicitPathOptions);
            }
        }
        #endregion

        #region Private Methods
        #endregion
    }
}
