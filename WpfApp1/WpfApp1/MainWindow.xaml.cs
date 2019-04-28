using System.Windows;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ServiceReference1.ConfigurationSettingsServiceSoapClient client = new ServiceReference1.ConfigurationSettingsServiceSoapClient();
            WpfApp1.ServiceReference1.RequestHeader requestHeader = new ServiceReference1.RequestHeader();
            requestHeader.Id = "";
            requestHeader.UseDisambiguatedIdentityString = true;

            var size = client.GetMaxAttachmentSize(requestHeader);
        }
    }
}
