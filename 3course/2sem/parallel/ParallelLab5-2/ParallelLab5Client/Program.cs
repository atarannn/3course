using System;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ParallelLab5Client
{
    public class SocketClient
    {
        public static int Main(string[] args)
        {
            StartClient();
            return 0;
        }


        private static void StartClient()
        {
            var bytes = new byte[1024];

            try
            {
                // Connect to a Remote server  
                // Get Host IP Address that is used to establish a connection  
                // In this case, we get one IP address of localhost that is IP : 127.0.0.1  
                // If a host has multiple addresses, you will get a list of addresses  
                var host = Dns.GetHostEntry("localhost");
                var ipAddress = host.AddressList.FirstOrDefault()
                                ?? throw new Exception("Can`t get ip address");
                var remoteEp = new IPEndPoint(ipAddress, 11000);

                // Create a TCP/IP  socket.    
                var sender = new Socket(ipAddress.AddressFamily,
                    SocketType.Stream, ProtocolType.Tcp);

                // Connect to Remote EndPoint  
                sender.Connect(remoteEp);

                Console.WriteLine($"Socket connected to {sender.RemoteEndPoint}");

                // Receive the response from the remote device.    
                var bytesRec = sender.Receive(bytes);
                Console.WriteLine($"SONNET:\n {Encoding.UTF8.GetString(bytes, 0, bytesRec)}");

                // Release the socket.    
                sender.Shutdown(SocketShutdown.Both);
                sender.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}