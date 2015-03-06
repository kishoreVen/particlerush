using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

using Newtonsoft.Json;

namespace FileStructureGenerator
{
    public class AssetData : IComparable<AssetData>
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

    public class AssetManager
    {
        #region Private Variables
        private List<AssetData> mAssetList;
        #endregion

        #region Public Variables 
        public List<AssetData> AssetList
        {
            get
            {
                UpdateAssetList();

                return mAssetList;
            }
        }
        #endregion

        #region Constructor
        public AssetManager()
        {
            UpdateAssetList();
        }
        #endregion

        #region Private Methods
        private void UpdateAssetList()
        {
            try
            {
                if(Globals.RepositoryRoot.Length != 0)
                {
                    string jsonList = File.ReadAllText(Globals.RepositoryRoot + Globals.gameAssetListFileName);

                    mAssetList = JsonConvert.DeserializeObject<List<AssetData>>(jsonList);

                    mAssetList.Sort();
                }                
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.Message + "\nPlease contact Tool Developer.", "Asset Manager Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        #endregion

        #region Public Methods
        public bool AddAsset(string assetName, string assetFriendlyName, string assetTags)
        {
            UpdateAssetList();

            if (!DoesAssetExist(assetName))
            {
               if(File.Exists(Globals.RepositoryRoot + Globals.gameAssetListFileName))
               {
                    AssetData newAssetData = new AssetData();

                    newAssetData.assetName = assetName;
                    newAssetData.assetFriendlyName = assetFriendlyName;
                    newAssetData.assetTag = assetTags;

                    mAssetList.Add(newAssetData);
                    mAssetList.Sort();

                    string assetJson = JsonConvert.SerializeObject(mAssetList);

                    File.WriteAllText(Globals.RepositoryRoot + Globals.gameAssetListFileName, assetJson);
                }
                else
                {
                    System.Windows.MessageBox.Show("Asset List file does not exist. Please check with the developer to make sure first commit has been made.", "Asset Manager File does not Exist", MessageBoxButton.OK, MessageBoxImage.Error);

                    return false;
                }
            }
            else
            {
                System.Windows.MessageBox.Show("The asset you are trying to create already exists. Please sync Git to view it in your local system.", "Asset Already Exists", MessageBoxButton.OK, MessageBoxImage.Error);

                return false;
            }

            return true;
        }

        public bool DoesAssetExist(string assetName)
        {
            foreach(AssetData data in mAssetList)
            {
                if(data.assetName.Equals(assetName, StringComparison.InvariantCultureIgnoreCase))
                {
                    return true;
                }
            }

            return false;            
        }        

        public void RemoveAsset(string assetName)
        {
            UpdateAssetList();

            AssetData dataToRemove = null;

            foreach (AssetData data in mAssetList)
            {
                if (data.assetName == assetName)
                {
                    dataToRemove = data;

                    break;
                }
            }

            if(dataToRemove != null)
            {
                mAssetList.Remove(dataToRemove);
            }
        }

        public string[] GetMismatchesFromList(string[] assetListToCheckAgainst)
        {
            UpdateAssetList();

            List<string> assetMismatchList = new List<string>();

            foreach(string asset in assetListToCheckAgainst)
            {
                if(!DoesAssetExist(asset))
                {
                    assetMismatchList.Add(asset);
                }
            }

            return assetMismatchList.ToArray();
        }
        #endregion
    }
}
