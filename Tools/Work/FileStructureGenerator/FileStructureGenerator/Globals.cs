using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileStructureGenerator
{
    public class Globals
    {
        #region App Consts
        public const string rootNotSelected = "Not Selected (Click Here To Select)";
        public const string assetNamePlaceHolder = "Asset Name; Asset Category";
        public const string assetTypePlaceHolder = "Asset Type; Type Prefix; Type Category";
        public const string selectAssetName = "----Select Asset Name----";
        public const string selectAssetType = "----Select Asset Type----";
        public const string modifierNotRequired = "Modifier Not Required";

        public const string particleRushTag = "## Red Tea Games - Particle Rush ##";

        public const string assetExplorerAllCategory = "All Assets";
        public const string assetTypeExplorerAllCategory = "All Asset Types";

        public const string uncategorizedExplorerCategory = "Unknown Category";

        public const string finalAssetNameLabel = "Name: ";
        public const string finalAssetPrefixLabel = "Prefix: ";
        public const string finalModifierPlaceholder = "Add Modifier";
        public const string finalAssetFileLabel = "File Name: ";

        public const string btnToSettingsPage = "Settings";
        public const string btnToMainPage = "Back";
        #endregion

        #region Path Consts
        public const string gameAssetsFolderName = "\\GameAssets\\";
        public const string uiAssetsFolderName = "\\UIAssets\\";

        public const string dataFolderName = "\\Data\\";
        public const string gameAssetListFileName = dataFolderName + "AssetList.json";
        public const string fileChartLocation = dataFolderName + "FileChart.json";

        public const string localFolderName = "Local\\";
        public const string configFileName = "Config.json";

        public const string gitIgnoreFile = ".gitignore";

        public const string unrealWorkFolder = "\\Work\\Unreal\\ParticleRush\\";
        public const string unrealImportFolder = "Import\\";
        public const string unrealImportTestFolder = "Test\\";
        public const string unrealImportFinalFolder = "Final\\";

        public const string batchFilesFolder = "\\Tools\\BatchFiles\\";        
        #endregion        

        #region App Settings
        public static string RepositoryRoot
        {
            get
            {
                return ConfigurationManager.GetConfigValue("RepositoryRoot");
            }
            set
            {
                ConfigurationManager.UpdateConfigValue("RepositoryRoot", value);
            }
        }

        public static string BitbucketUsername
        {
            get
            {
                return ConfigurationManager.GetConfigValue("GitUsername");
            }
            set
            {
                ConfigurationManager.UpdateConfigValue("GitUsername", value);
            }
        }

        public static string BitbucketPassword
        {
            get
            {
                return ConfigurationManager.GetConfigValue("GitPassword");
            }
            set
            {
                ConfigurationManager.UpdateConfigValue("GitPassword", value);
            }
        }

        public static string BitbucketEmail
        {
            get
            {
                return ConfigurationManager.GetConfigValue("GitEmail");
            }
            set
            {
                ConfigurationManager.UpdateConfigValue("GitEmail", value);
            }
        }

        public static string BitbucketOwner
        {
            get
            {
                return "kishore_venkateshan";
            }
        }

        public static string BitbucketRepository
        {
            get
            {
                return "ParticleRush";
            }            
        }

        public static string BitbucketGitUrl
        {
            get
            {
                return "https://kishore_venkateshan@bitbucket.org/kishore_venkateshan/particlerush.git";
            }
        }

        public static string BitbucketAutheticationUrl
        {
            get
            {
                return "https://" + BitbucketUsername + ":" + BitbucketPassword + "@bitbucket.org/" + BitbucketUsername + "/particlerush.git";
            }
        }
        #endregion
    }
}
