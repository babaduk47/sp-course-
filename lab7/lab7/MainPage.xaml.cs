using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Media.Animation;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace lab7
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
            MyFrame.Navigate(typeof(FuelConsumption), null, new DrillInNavigationTransitionInfo());
            Header.Text = "Конвертатор расхода топлива";
        }

        private void Gamburger_Click(object sender, RoutedEventArgs e)
        {
            MySplitView.IsPaneOpen = !MySplitView.IsPaneOpen;
        }

        private void MyListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ItemArea.IsSelected)
            {
                MyFrame.Navigate(typeof(Area), null, new DrillInNavigationTransitionInfo());
                Header.Text = "Конвертатор площадей";
            }
            if (ItemFuelConsumption.IsSelected)
            {
                MyFrame.Navigate(typeof(FuelConsumption), null, new DrillInNavigationTransitionInfo());
                Header.Text = "Конвертатор расхода топлива";
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            MyFrame.Navigate(typeof(About), null, new DrillInNavigationTransitionInfo());
            Header.Text = "О Себе";
        }
    }
}
