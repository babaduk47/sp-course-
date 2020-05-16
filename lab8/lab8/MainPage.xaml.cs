using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.Storage.FileProperties;
using Windows.Storage.Search;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace lab8
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {

        StorageFolder prevLocation = null;
        StorageFolder curFolder = Windows.ApplicationModel.Package.Current.InstalledLocation;
        private ObservableCollection<Item> filelist = new ObservableCollection<Item>();
        public StorageFolder CurrentFolder
        {
            get
            {
                return curFolder;
            }

            set
            {
                prevLocation = curFolder;
                curFolder = value;
            }
        }
        
        public MainPage()
        {
            this.InitializeComponent();
            listView.ItemsSource = filelist;
            listView.ItemClick += ListView_ItemClick;
            PrintDir();
        }

        private void ListView_ItemClick(object sender, ItemClickEventArgs e)
        {
            foreach (Item item in filelist)
            {
                if (e.ClickedItem.ToString() == item.Filename && item.Folder != null)
                {
                    CurrentFolder = item.Folder;
                    PrintDir();
                    break;
                }
            }
        }

        async private void PrintDir()
        {
            filelist.Clear();
            if (CurrentFolder == null) return;
            StorageFolder parent = await CurrentFolder.GetParentAsync();
            if (parent != null)
            {
                searchTextbox.Text = "";
                filelist.Add(new Item { Filename = "..", Size = "FOLDER", Folder = parent });
            }
            else
            {
                if (prevLocation != null)
                {
                    filelist.Add(new Item { Filename = "НАЗАД", Size = "FOLDER", Folder = prevLocation });
                }
            }
            IReadOnlyList<StorageFolder> folderList = await CurrentFolder.GetFoldersAsync();
            foreach (StorageFolder folder in folderList)
            {
                filelist.Add(new Item { Filename = folder.DisplayName, Size = "FOLDER", Folder = folder });
            }
            IReadOnlyList<StorageFile> fileList = await CurrentFolder.GetFilesAsync();
            foreach (StorageFile file in fileList)
            {
                Windows.Storage.FileProperties.BasicProperties basicProperties = await file.GetBasicPropertiesAsync();
                string fileSize = ParseSize(basicProperties.Size);
                filelist.Add(new Item { Filename = file.Name, Size = fileSize, File = file });
            }
            for (int i = 0; i < filelist.Count; ++i)
            {
                if (filelist[i].Filename == ".." || filelist[i].Filename == "НАЗАД")
                {
                    Item item = filelist[i];
                    filelist.RemoveAt(i);
                    filelist.Insert(0, item);
                }
            }
            return;
        }

        private string ParseSize(ulong bytes)
        {
            double size = bytes;
            uint divided = 0;
            string result = "";

            while (size >= 1024)
            {
                size /= 1024;
                divided += 1;
            }

            switch (divided)
            {
                case 0:
                    result = string.Format("{0:n0} B", size);
                    break;
                case 1:
                    result = string.Format("{0:n0} KB", size);
                    break;
                case 2:
                    result = string.Format("{0:n0} MB", size);
                    break;
                case 3:
                    result = string.Format("{0:n0} GB", size);
                    break;
                default:
                    break;
            }

            return result;
        }

        private void searchButton_Click(object sender, RoutedEventArgs e)
        {
            if (searchTextbox.Text == "")
            {
                return;
            }

            filelist.Clear();
            filelist.Add(new Item { Filename = "<-- Вернуться обратно", Size = "FOLDER", Folder = CurrentFolder });

            Search(CurrentFolder);
        }

        async private void Search(StorageFolder folder)
        {
            SearchFiles(folder);
            IReadOnlyList<StorageFolder> folderList = await folder.GetFoldersAsync();
            foreach (StorageFolder innerFolder in folderList)
            {
                Search(innerFolder);
            }
        }

        async private void SearchFiles(StorageFolder folder)
        {
            IReadOnlyList<StorageFile> fileList = await folder.GetFilesAsync();
            foreach (StorageFile file in fileList)
            {
                if (file.DisplayName.IndexOf(searchTextbox.Text) >= 0)
                {
                    Windows.Storage.FileProperties.BasicProperties basicProperties = await file.GetBasicPropertiesAsync();
                    string fileSize = ParseSize(basicProperties.Size);
                    filelist.Add(new Item { Filename = (file.Name), Size = fileSize, File = file });
                }
            }
        }

        async private void cdButton_Click(object sender, RoutedEventArgs e)
        {
            var folderPicker = new Windows.Storage.Pickers.FolderPicker();
            folderPicker.SuggestedStartLocation = Windows.Storage.Pickers.PickerLocationId.Desktop;
            folderPicker.FileTypeFilter.Add("*");
            Windows.Storage.StorageFolder folder = await folderPicker.PickSingleFolderAsync();
            if (folder != null)
            {
                CurrentFolder = folder;
                PrintDir();
            }
        }
    }
    public class Item
    {
        public string Filename { get; set; }
        public string Size { get; set; }
        public StorageFolder Folder { get; set; }
        public StorageFile File { get; set; }
        public override string ToString()
        {
            return Filename;
        }
    }
}
