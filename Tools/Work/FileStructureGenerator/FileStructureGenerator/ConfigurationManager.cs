using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Newtonsoft.Json;

namespace FileStructureGenerator
{
    class ConfigurationData
    {
        [JsonProperty(PropertyName = "configName")]
        public string configName
        {
            get;
            set;            
        }

        [JsonProperty(PropertyName = "configValue")]
        public string configValue
        {
            get;
            set;
        }
    }


    public static class ConfigurationManager
    {
        #region Private Variables

        #endregion

        #region Public Variables
        public static bool IsFirstRun;
        #endregion

        #region Constructor
        static ConfigurationManager()
        {
            if(!Directory.Exists(Globals.localFolderName))
            {
                Directory.CreateDirectory("Local");

                IsFirstRun = true;
            }
                
            if(!File.Exists(Globals.localFolderName + Globals.configFileName))
            {
                string firstLines = "[]";
                
                File.WriteAllText(Globals.localFolderName + Globals.configFileName, firstLines);                

                IsFirstRun = true;
            }
            else
            {
                IsFirstRun = false;
            }            
        }
        #endregion

        #region Methods
        public static void UpdateConfigValue(string configName, string configValue)
        {
            bool wasExistingValueUpdated = false;

            string fullConfigString = File.ReadAllText(Globals.localFolderName + Globals.configFileName);

            List<ConfigurationData> configList = JsonConvert.DeserializeObject<List<ConfigurationData>>(fullConfigString);

            foreach(ConfigurationData config in configList)
            {
                if (configName == config.configName)
                {
                    config.configValue = configValue;

                    wasExistingValueUpdated = true;
                }
            }

            if(wasExistingValueUpdated == false)
            {
                /* Should the configuration not exist */
                ConfigurationData newConfig = new ConfigurationData();
                newConfig.configName = configName;
                newConfig.configValue = configValue;

                configList.Add(newConfig);
            }
            
            /* Update the JSON */
            string configJson = JsonConvert.SerializeObject(configList);

            File.WriteAllText(Globals.localFolderName + Globals.configFileName, configJson);
        }

        public static string GetConfigValue(string configName)
        {
            string configValue = "";

            string fullConfigString = File.ReadAllText(Globals.localFolderName + Globals.configFileName);

            List<ConfigurationData> configList = JsonConvert.DeserializeObject<List<ConfigurationData>>(fullConfigString);

            foreach(ConfigurationData config in configList)
            {
                if(configName == config.configName)
                {
                    configValue = config.configValue; 
                }
            }

            return configValue;
        }

        public static bool CheckParticleRushGitRoot(string folderPath)
        {
            if (Directory.Exists(folderPath))
            {
                if(File.Exists(folderPath + "\\" + Globals.gitIgnoreFile))
                {
                    string[] gitIgnoreLines = File.ReadAllLines(folderPath + "\\" + Globals.gitIgnoreFile);

                    if(Globals.particleRushTag == gitIgnoreLines[0])
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        public static bool IsAdmin()
        {
            return Globals.BitbucketUsername.Equals(Globals.BitbucketOwner);            
        }
        #endregion
    }
}
