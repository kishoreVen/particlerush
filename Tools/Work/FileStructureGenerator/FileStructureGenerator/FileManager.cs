using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.IO;
using System.Windows;

namespace FileStructureGenerator
{
    public class FileData : IComparable<FileData>
    {
        [JsonProperty(PropertyName = "fileType")]
        public string fileType
        {
            get;
            set;
        }

        [JsonProperty(PropertyName = "fileFriendlyType")]
        public string fileFriendlyType
        {
            get;
            set;
        }

        [JsonProperty(PropertyName = "filePrefix")]
        public string filePrefix
        {
            get;
            set;
        }

        [JsonProperty(PropertyName = "fileTag")]
        public string fileTag
        {
            get;
            set;
        }

        public int CompareTo(FileData other)
        {
            return this.fileType.CompareTo(other.fileType);
        }

        public static int CompareByTags(FileData a, FileData b)
        {
            return a.fileTag.CompareTo(b.fileTag);
        }
    }

    public class FileManager
    {
        #region Private Variables
        private List<FileData> mFileChart;
        #endregion

        #region Public Variables
        public List<FileData> FileChart
        {
            get
            {
                UpdateFileChart();

                return mFileChart;
            }
        }
        #endregion

        #region Constructor
        #endregion

        #region Methods
        private void UpdateFileChart()
        {
            if(Globals.RepositoryRoot.Length != 0)
            {
                if (File.Exists(Globals.RepositoryRoot + Globals.fileChartLocation))
                {
                    /* Get JSON string from file */
                    string jsonString = File.ReadAllText(Globals.RepositoryRoot + Globals.fileChartLocation);

                    /* Convert string to file chart list */
                    mFileChart = JsonConvert.DeserializeObject<List<FileData>>(jsonString);
                }
                else
                {
                    System.Windows.MessageBox.Show("File Chart doesn't Exist or hasn't been synced. Please contact Tool Developer.", "File Chart doesn't Exist", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }            
        }

        public bool AddNewFileType(string typeName, string typeFriendlyName, string typePrexix, string typeTag)
        {
            FileData newFileData = new FileData();
            newFileData.fileType = typeName;
            newFileData.fileFriendlyType = typeFriendlyName;
            newFileData.filePrefix = typePrexix;
            newFileData.fileTag = typeTag;

            foreach (FileData fileType in mFileChart)
            {
                if(newFileData.fileType == fileType.fileType)
                {
                    return false;
                }
            }

            mFileChart.Add(newFileData);

            mFileChart.Sort();

            string newJsonString = JsonConvert.SerializeObject(mFileChart);

            File.WriteAllText(Globals.RepositoryRoot + Globals.fileChartLocation, newJsonString);

            UpdateFileChart();

            return true;
        }
    
        public void RemoveExistingFileType(string existingFileTypeID)
        {
            UpdateFileChart();

            FileData fileTypeToRemove = null;

            foreach (FileData fileType in mFileChart)
            {
                if (fileType.fileType == existingFileTypeID)
                {
                    fileTypeToRemove = fileType;
                }
            }

            if (fileTypeToRemove != null)
            {
                mFileChart.Remove(fileTypeToRemove);
            }            
        }
    
        public FileData GetFileType(string fileTypeName)
        {
            foreach (FileData fileType in mFileChart)
            {
                if (fileType.fileType == fileTypeName)
                {
                    return fileType;
                }
            }

            return null;
        }

        public bool DoesFileTypeIdExist(string friendlyTypeName)
        {
            foreach (FileData fileType in mFileChart)
            {
                if (fileType.fileFriendlyType == friendlyTypeName)
                {
                    return true;
                }
            }

            return false;
        }
        #endregion
    }
}
