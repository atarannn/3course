using System;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace ParallelLab5Server
{
    public class SocketListener
    {
        public static int Main(string[] args)
        {
            var task = Task.Run(StartServer);
            task.Wait();
            return 0;
        }


        private static async Task StartServer()
        {
            // Get Host IP Address that is used to establish a connection  
            // In this case, we get one IP address of localhost that is IP : 127.0.0.1  
            // If a host has multiple addresses, you will get a list of addresses  
            var host = await Dns.GetHostEntryAsync("localhost");
            var ipAddress = host.AddressList.FirstOrDefault()
                            ?? throw new Exception("Can`t get ip address");
            var localEndPoint = new IPEndPoint(ipAddress, 11000);
            var sonnets = File
                .ReadAllTextAsync("/home/stas/RiderProjects/ParallelLab5/ParallelLab5Server/sonnets.txt")
                .Result
                .Split("\n\n");
            var rnd = new Random();

            try
            {
                // Create a Socket that will use Tcp protocol      
                var listener = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
                // A Socket must be associated with an endpoint using the Bind method  
                listener.Bind(localEndPoint);
                listener.Listen();
                while (true)
                {
                    Console.WriteLine("Waiting for a connection...");
                    using var handler = await listener.AcceptAsync();
                    Console.WriteLine($"Received connection from {handler.RemoteEndPoint}");

                    var msg = Encoding.UTF8.GetBytes(sonnets[rnd.Next(sonnets.Length)]);
                    handler.Send(msg);
                    handler.Shutdown(SocketShutdown.Both);
                    handler.Close();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }

            Console.WriteLine("\n Press any key to continue...");
            Console.ReadKey();
        }
    }
}