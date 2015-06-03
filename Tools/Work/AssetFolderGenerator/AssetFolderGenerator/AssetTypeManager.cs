using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AssetFolderGenerator
{
    class AssetTypeData : IComparable<AssetTypeData>
    {
        [JsonProperty(PropertyName = "assetName")]
        public string assetTypeName
        {
            get;
            set;
        }

        [JsonProperty(PropertyName = "assetFriendlyName")]
        public string assetTypeFriendlyName
        {
            get;
            set;
        }

        [JsonProperty(PropertyName = "assetPrefix")]
        public string assetTypePrefix
        {
            get;
            set;
        }        

        public int CompareTo(AssetTypeData other)
        {
            return this.assetTypeName.CompareTo(other.assetTypeName);
        }

        public static int CompareByFriendlyNames(AssetTypeData a, AssetTypeData b)
        {
            return a.assetTypeFriendlyName.CompareTo(b.assetTypeFriendlyName);
        }
    }

    class AssetTypeManager
    {
        #region CONST
        const string ASSET_TYPE_META_FILE = "\\Data\\AssetTypeList.json";
        #endregion

        #region FIELDS
        private List<AssetTypeData>         mAssetTypeList;
        private ConfigurationManager        mConfigurationManager;
        private GitManager                  mGitManager;
        #endregion

        #region PROPERTIES
        public List<AssetTypeData> AssetTypeList
        {
            get
            {
                UpdateAssetTypeList();

                return mAssetTypeList;
            }
        }
        #endregion

        #region CONSTRUCTOR
        public AssetTypeManager(ConfigurationManager configurationManager, GitManager gitManager)
        {
            mConfigurationManager   = configurationManager;
            mGitManager             = gitManager;

            UpdateAssetTypeList();
        }
        #endregion

        #region METHODS
        private void UpdateAssetTypeList()
        {
            try
            {
                if ("" != mConfigurationManager.RepositoryRoot)
                {
                    string jsonList = File.ReadAllText(mConfigurationManager.RepositoryRoot + ASSET_TYPE_META_FILE);

                    mAssetTypeList = JsonConvert.DeserializeObject<List<AssetTypeData>>(jsonList);
                    mAssetTypeList.Sort();
                }
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.Message + "\nPlease contact Tool Developer.", "Asset Type Manager Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
            }
        }

        public bool DoesAssetTypeExist(string assetTypeName)
        {
            UpdateAssetTypeList();
            foreach (AssetTypeData data in mAssetTypeList)
            {
                if (data.assetTypeName.Equals(assetTypeName, StringComparison.InvariantCultureIgnoreCase))
                    return true;
            }

            return false;
        }

        public void RemoveAssetType(string assetTypeFriendlyName)
        {
            AssetTypeData dataToRemove = GetAsset(assetTypeFriendlyName);

            if (dataToRemove != null)
            {
                try
                {
                    mAssetTypeList.Remove(dataToRemove);

                    string assetJson = JsonConvert.SerializeObject(mAssetTypeList);
                    File.WriteAllText(mConfigurationManager.RepositoryRoot + ASSET_TYPE_META_FILE, assetJson);

                    #region File Commit
                    mGitManager.StageFileForCommit(mConfigurationManager.RepositoryRoot + ASSET_TYPE_META_FILE, "Removed Asset Type: " + assetTypeFriendlyName);
                    #endregion
                }
                catch(System.Exception ex)
                {
                    System.Windows.Forms.MessageBox.Show("Couldn't Remove Asset Type! \n" + ex.Message, "Asset Type Remove Failure!", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
                }
            }
        }

        public void AddAssetType()
        {
            string input = Microsoft.VisualBasic.Interaction.InputBox("Prompt", "Input Asset Type: (Name;Friendly Name;Prefix)", "", 0, 0);

            string[] inputSplit = input.Split(';');

            while(inputSplit.Length != 3)
                input = Microsoft.VisualBasic.Interaction.InputBox("Prompt", "Input Asset Type: (Name;Friendly Name;Prefix)", "", 0, 0);

            string assetTypeName                = inputSplit[0];
            string assetTypeFriendlyName    = inputSplit[1];
            string assetTypePrefix              = inputSplit[2];

            string assetMetaFileLocation = mConfigurationManager.RepositoryRoot + ASSET_TYPE_META_FILE;

            if (!DoesAssetTypeExist(assetTypeName))
            {
                if (File.Exists(assetMetaFileLocation))
                {
                    AssetTypeData newAssetData = new AssetTypeData();

                    newAssetData.assetTypeName = assetTypeName;
                    newAssetData.assetTypeFriendlyName = assetTypeFriendlyName;
                    newAssetData.assetTypePrefix = assetTypePrefix;

                    mAssetTypeList.Add(newAssetData);
                    mAssetTypeList.Sort(AssetTypeData.CompareByFriendlyNames);

                    string assetJson = JsonConvert.SerializeObject(mAssetTypeList);
                    File.WriteAllText(assetMetaFileLocation, assetJson);

                    #region File Commit
                    mGitManager.StageFileForCommit(mConfigurationManager.RepositoryRoot + ASSET_TYPE_META_FILE, "Added Asset Type: " + assetTypeName);
                    #endregion
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

        public AssetTypeData GetAsset(string assetTypeFriendlyName)
        {
            UpdateAssetTypeList();

            foreach (AssetTypeData assetType in mAssetTypeList)
            {
                if (assetType.assetTypeFriendlyName == assetTypeFriendlyName)
                    return assetType;
            }

            return null;
        }
        #endregion
    }
}
