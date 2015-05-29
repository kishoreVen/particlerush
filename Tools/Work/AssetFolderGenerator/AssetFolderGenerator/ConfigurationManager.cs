using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using System.IO;
using System.Windows.Forms;

namespace AssetFolderGenerator
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


    class ConfigurationManager
    {
        #region CONST
        private const string LOCAL_CONFIGURATION_FOLDER = "Local\\";
        private const string CONFIG_FILE = "Config.json";
        private const string GIT_IGNORE_FILE = ".gitignore";
        private const string PARTICLE_RUSH_TAG = "## Red Tea Games - Particle Rush ##";
        #endregion

        #region FIELDS
        private static string _configLocation = LOCAL_CONFIGURATION_FOLDER + CONFIG_FILE;
        #endregion

        #region PROPERTIES
        public string RepositoryRoot
        {
            get
            {
                return GetConfigValue("RepositoryRoot");
            }
            set
            {
                UpdateConfigValue("RepositoryRoot", value);
            }
        }

        public string GitUsername
        {
            get
            {
                return GetConfigValue("GitUsername");
            }
            set
            {
                UpdateConfigValue("GitUsername", value);
            }
        }

        public string GitPassword
        {
            get
            {
                return GetConfigValue("GitPassword");
            }
            set
            {
                UpdateConfigValue("GitPassword", value);
            }
        }
        #endregion

        #region CONSTRUCTOR
        public ConfigurationManager()
        {
            if (!Directory.Exists(LOCAL_CONFIGURATION_FOLDER))
            {
                Directory.CreateDirectory("Local");
            }

            if (!File.Exists(_configLocation))
            {
                string firstLines = "[]";

                File.WriteAllText(_configLocation, firstLines);
            }

            if ("" == RepositoryRoot || !CheckParticleRushGitRoot(RepositoryRoot))
            {
                RepositoryRoot = PromptForRepositoryRoot();
            }
        }
        #endregion

        #region METHOD
        private bool CheckParticleRushGitRoot(string folderPath)
        {
            string gitIgnoreLocation = folderPath + "\\" + GIT_IGNORE_FILE;
            if (Directory.Exists(folderPath))
            {
                if (File.Exists(gitIgnoreLocation))
                {
                    string[] gitIgnoreLines = File.ReadAllLines(gitIgnoreLocation);

                    if (PARTICLE_RUSH_TAG == gitIgnoreLines[0])
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        public void UpdateConfigValue(string configName, string configValue)
        {
            bool wasExistingValueUpdated = false;

            string fullConfigString = File.ReadAllText(_configLocation);
            List<ConfigurationData> configList = JsonConvert.DeserializeObject<List<ConfigurationData>>(fullConfigString);

            foreach (ConfigurationData config in configList)
            {
                if (configName == config.configName)
                {
                    config.configValue = configValue;
                    wasExistingValueUpdated = true;
                }
            }

            if (!wasExistingValueUpdated)
            {
                /* Should the configuration not exist */
                ConfigurationData newConfig = new ConfigurationData();
                newConfig.configName = configName;
                newConfig.configValue = configValue;

                configList.Add(newConfig);
            }

            /* Update the JSON */
            string configJson = JsonConvert.SerializeObject(configList);
            File.WriteAllText(_configLocation, configJson);
        }

        public string GetConfigValue(string configName)
        {
            string configValue = "";

            string fullConfigString = File.ReadAllText(_configLocation);
            List<ConfigurationData> configList = JsonConvert.DeserializeObject<List<ConfigurationData>>(fullConfigString);

            foreach (ConfigurationData config in configList)
            {
                if (configName == config.configName)
                {
                    configValue = config.configValue;
                }
            }

            return configValue;
        }

        public string PromptForRepositoryRoot()
        {
            FolderBrowserDialog rootPicker = new FolderBrowserDialog();
            rootPicker.Description = "Pick the root directory of repository you cloned from git.";

            while (true)
            {
                DialogResult pickResult = rootPicker.ShowDialog();
                if (pickResult == System.Windows.Forms.DialogResult.OK)
                {
                    if (!CheckParticleRushGitRoot(rootPicker.SelectedPath))
                    {
                        System.Windows.Forms.MessageBox.Show("The path you selected is not the root folder of particle rush repository. Please select the correct folder.", "Not particle rush repository", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
                        continue;
                    }
                    else
                    {
                        // Update the Folder Picker
                        return rootPicker.SelectedPath;
                    }
                }
                else
                {
                    break;
                }

            }

            return "";
        }
        #endregion
    }
}
