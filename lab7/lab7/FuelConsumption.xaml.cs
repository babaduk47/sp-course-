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
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace lab7
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class FuelConsumption : Page
    {
        double answ = 0;
        double inp = 0;
        public FuelConsumption()
        {
            this.InitializeComponent();
            main();

        }

        void main()
        {
            if (!double.TryParse(MyInput.Text, out inp))
            {
                MyError.Visibility = Visibility.Visible;
                MyError.Text = "Не число.Ошибка ввода";
                MyAnswer.Text = "";
            }
            else
            {
                if (Calculate(inp, out answ))
                {
                    MyError.Visibility = Visibility.Collapsed;
                    MyAnswer.Text = answ.ToString();
                }
            }
        }



        bool Calculate(double x,out double result)
        {
            result = 0;
            double gal = 235.215;
            if (x == 0)
            {
                return false;
            }
            result = gal / x;
            return true;
        }

        private void MyInput_TextChanged(object sender, TextChangedEventArgs e)
        {
            main();
        }
    }
}
