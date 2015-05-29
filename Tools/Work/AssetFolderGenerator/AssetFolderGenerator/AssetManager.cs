using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using System.IO;
using System.Diagnostics;

namespace AssetFolderGenerator
{
    class AssetData : IComparable<AssetData>
    {
        [JsonProperty(PropertyName = "assetName")]
        public string assetName
        {
            get;
            set;
        }

        [JsonProperty(PropertyName = "assetFriendlyName")]
        public string assetFriendlyName
        {
            get;
            set;
        }

        [JsonProperty(PropertyName = "assetTag")]
        public string assetTag
        {
            get;
            set;
        }

        public int CompareTo(AssetData other)
        {
            return this.assetName.CompareTo(other.assetName);
        }

        public static int CompareByTags(AssetData a, AssetData b)
        {
            return a.assetTag.CompareTo(b.assetTag);
        }
    }

    class AssetManager
    {
        #region CONST
        const string ASSET_META_FILE = "\\Data\\AssetList.json";
        const string ASSET_FOLDER = "\\GameAssets\\";
        const string UNREAL_IMPORT_FOLDER = "\\Work\\Unreal\\ParticleRush\\Import\\";
        #endregion

        #region FIELDS
        private List<AssetData>         mAssetList;
        private ConfigurationManager    mConfigurationManager;
        private GitManager              mGitManager;
        #endregion

        #region PUBLIC
        public List<AssetData> AssetList
        {
            get
            {
                UpdateAssetList();

                return mAssetList;
            }
        }
        #endregion

        #region CONSTRUCTOR
        public AssetManager(ConfigurationManager configurationManager, GitManager gitManager)
        {
            mConfigurationManager   = configurationManager;
            mGitManager             = gitManager;

            UpdateAssetList();
        }
        #endregion

        #region METHODS
        private void UpdateAssetList()
        {
            try
            {
                if ("" != mConfigurationManager.RepositoryRoot)
                {
                    string jsonList = File.ReadAllText(mConfigurationManager.RepositoryRoot + ASSET_META_FILE);

                    mAssetList = JsonConvert.DeserializeObject<List<AssetData>>(jsonList);
                    mAssetList.Sort();
                }
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.Message + "\nPlease contact Tool Developer.", "Asset Manager Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
            }
        }

        private void CreateDirectory(string assetName)
        {
            CreateArtDirectoryStructure(assetName);
            CreateUnrealImportDirectoryStructure(assetName);
        }

        private void CreateArtDirectoryStructure(string assetName)
        {
            string assetFolder = mConfigurationManager.RepositoryRoot + ASSET_FOLDER + assetName;

            string conceptFolder = assetFolder + "\\Concept";
            string assetExportFolder = assetFolder + "\\AssetExport";
            string texturesFolder = assetExportFolder + "\\Textures";
            string audioFolder = assetFolder + "\\Audio";

            try
            {
                // Create Asset Root Directory
                Directory.CreateDirectory(assetFolder);
                File.WriteAllText(assetFolder + "\\Readme.txt", assetName + " -- Main Asset Folder");

                // Child Directories
                Directory.CreateDirectory(conceptFolder);
                File.WriteAllText(conceptFolder + "\\Readme.txt", assetName + " -- Concept Folder");
                Directory.CreateDirectory(assetExportFolder);
                File.WriteAllText(assetExportFolder + "\\Readme.txt", assetName + " -- Main Asset Export Folder");

                // Child Sub Directories

                Directory.CreateDirectory(texturesFolder);
                File.WriteAllText(texturesFolder + "\\Readme.txt", assetName + " -- Textures Folder For Artist");

                System.Windows.Forms.DialogResult confirmation = System.Windows.Forms.MessageBox.Show("The Structure has been successfully created at " + assetFolder + ". Do you wish to open the folder in explorer?", "Structure Creation Successful", System.Windows.Forms.MessageBoxButtons.YesNo, System.Windows.Forms.MessageBoxIcon.Information);

                if (confirmation == System.Windows.Forms.DialogResult.Yes)
                {
                   Process.Start("explorer.exe", assetFolder);
                }

                #region Directory Commit
                mGitManager.StageDirectoryForCommit(assetFolder, "Added Game Asset Structure for " + assetName);
                #endregion
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.Message, "Structure Creation Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
            }
        }

        private void CreateUnrealImportDirectoryStructure(string assetName)
        {
            string assetFolder = mConfigurationManager.RepositoryRoot + UNREAL_IMPORT_FOLDER + assetName;
            string animDirectory = assetFolder + "\\Anims";
            try
            {
                // Create Asset Root Directory
                Directory.CreateDirectory(assetFolder);
                // Create ReadMe structure for model
                File.WriteAllText(assetFolder + "\\ReadMe.txt", "## Asset Name: " + assetName + " ##");

                // Create Directory for animations
                Directory.CreateDirectory(animDirectory);
                // Establish Temp file for folder commit
                File.WriteAllText(animDirectory + "\\AnimDetails.txt", "## Anim No. - Animation Name - Frame Start, Frame End ##");

                #region Directory Commit
                mGitManager.StageDirectoryForCommit(assetFolder, "Added Import directory structure for " + assetName);
                #endregion
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.Message, "Structure Creation Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
            }
        }

        private void RemoveDirectory(string assetName)
        {
            string gameAssetFolder  = mConfigurationManager.RepositoryRoot + ASSET_FOLDER + assetName;
            string gameImportFolder = mConfigurationManager.RepositoryRoot + UNREAL_IMPORT_FOLDER + assetName;

            if(Directory.Exists(gameAssetFolder))
            {
                Directory.Delete(gameAssetFolder, true);

                #region Directory Commit
                mGitManager.StageDirectoryForCommit(gameAssetFolder, "Removed Game Asset Directory structure for " + assetName);                
                #endregion
            }

            if (Directory.Exists(gameImportFolder))
            {
                Directory.Delete(gameImportFolder, true);

                #region Directory Commit
                mGitManager.StageDirectoryForCommit(gameImportFolder, "Removed Import Directory structure for " + assetName);
                #endregion
            }
        }

        public bool DoesAssetExist(string assetName)
        {
            UpdateAssetList();
            foreach (AssetData data in mAssetList)
            {
                if (data.assetName.Equals(assetName, StringComparison.InvariantCultureIgnoreCase))
                    return true;                
            }

            return false;
        }

        public void RemoveAsset(string assetName)
        {
            AssetData dataToRemove = null;

            UpdateAssetList();
            foreach (AssetData data in mAssetList)
            {
                if (data.assetName == assetName)
                {
                    dataToRemove = data;
                    break;
                }
            }

            if (dataToRemove != null)
            {
                mAssetList.Remove(dataToRemove);
                RemoveDirectory(assetName);

                string assetJson = JsonConvert.SerializeObject(mAssetList);
                File.WriteAllText(mConfigurationManager.RepositoryRoot + ASSET_META_FILE, assetJson);

                #region File Commit
                mGitManager.StageFileForCommit(mConfigurationManager.RepositoryRoot + ASSET_META_FILE, "Removed Asset: " + assetName);
                #endregion
            }
        }

        public void AddAsset(string assetName, string assetFriendlyName, string assetTags)
        {
            string assetMetaFileLocation = mConfigurationManager.RepositoryRoot + ASSET_META_FILE;

            if (!DoesAssetExist(assetName))
            {
                if (File.Exists(assetMetaFileLocation))
                {
                    AssetData newAssetData = new AssetData();

                    newAssetData.assetName = assetName;
                    newAssetData.assetFriendlyName = assetFriendlyName;
                    newAssetData.assetTag = assetTags;

                    mAssetList.Add(newAssetData);
                    mAssetList.Sort();

                    string assetJson = JsonConvert.SerializeObject(mAssetList);
                    File.WriteAllText(assetMetaFileLocation, assetJson);

                    #region File Commit
                    mGitManager.StageFileForCommit(mConfigurationManager.RepositoryRoot + ASSET_META_FILE, "Added Asset: " + assetName);
                    #endregion

                    CreateDirectory(assetName);
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("Asset List file does not exist. Please check with the developer to make sure first commit has been made.", "Asset Manager File does not Exist", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
                }
            }
            else
            {
                System.Windows.Forms.MessageBox.Show("The asset you are trying to create already exists. Please sync Git to view it in your local system.", "Asset Already Exists", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
            }
        }

        public void ShowArtAssetInExplorer(string assetName)
        {
            string assetFolder = mConfigurationManager.RepositoryRoot + ASSET_FOLDER + assetName;

            Process.Start("explorer.exe", assetFolder);
        }

        public void ShowImportAssetInExplorer(string assetName)
        {
            string assetFolder = mConfigurationManager.RepositoryRoot + UNREAL_IMPORT_FOLDER + assetName;

            Process.Start("explorer.exe", assetFolder);
        }
        #endregion
    }
}
