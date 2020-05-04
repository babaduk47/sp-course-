using System;
using System.Collections.Generic;
using System.Globalization;
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
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace lab7
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class Area : Page
    {
        bool start = false;
        public Area()
        {
            this.InitializeComponent();
            start = true;
        }

        double inp = 0;
        double answ = 0;

        void main()
        {

            int From = FromArea.SelectedIndex;
            int To = ToArea.SelectedIndex;
            answ = 0;
            if (!double.TryParse(MyInput.Text, out inp))
            {
                MyError.Visibility = Visibility.Visible;
                MyError.Text = "Не число.Ошибка ввода";
                MyAnswer.Text = "";
                return;
            }
            else MyError.Visibility = Visibility.Collapsed;
            switch (From)
            {
                case 0:
                    switch (To)
                    {
                        case 0:
                            answ = inp * 1.1959900463011;
                            break;
                        case 1:
                            answ = inp / 2589988.110336;
                            break;
                    }
                    break;
                case 1:
                    switch (To)
                    {
                        case 0:
                            answ = inp * 1.196e+6;
                            break;
                        case 1:
                            answ = inp / 2.59;
                            break;
                    }
                    break;
                case 2:
                    switch (To)
                    {
                        case 0:
                            answ = inp * 11959.900463011;
                            break;
                        case 1:
                            answ = inp / 258.9988110336;
                            break;
                    }
                    break;
            }
            MyAnswer.Text = answ.ToString("G5").TrimEnd('0');
        }
        private void ToArea_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (!start) return;
            main();
        }

        private void FromArea_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (!start) return;
            main();
        }

        private void MyInput_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (!start) return;
            main();
        }
    }
}
