import java.net.*;
import java.io.*;

public class Client {
    public static void main(String[] args) throws IOException {
        while (true) {
            String hostName = "127.0.0.1"; //  IP-адрес хоста, который используется для установления соединения (localhost)
            int portNumber = 11000; //номер порта

            try (
                    Socket Socket = new Socket(hostName, portNumber); // сокет соединения по IP-адресу и номеру порта
                    PrintWriter out = new PrintWriter(Socket.getOutputStream(), true); // поток записи в сокет
                    BufferedReader in = new BufferedReader(new InputStreamReader(Socket.getInputStream())); // поток чтения из сокета
                    BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in)) // системный поток вывода
            ) {
                String userInput;
                out.println("help"); // отправляем на сервер запрос команды help (списки команд и пример ввода строки)
                System.out.println(in.readLine()); // получаем строку сервера и выводим в консоль

                while ((userInput = stdIn.readLine()) != null) { // считываем строку, которую ввели в консоль
                    out.println(userInput); // max [1,1,1,2,3,44,4,5,6,7,4] // отправляем на сервер строку
                    System.out.println(in.readLine()); // получаем строку с сервера и выводим в консоль
                }
            }
            catch (UnknownHostException e) { // исключение возникает, если класс сокета не смог преобразовать имя хоста в реальный, существующий, адрес
                System.err.println("Don't know about host " + hostName);

            }
            catch (IOException e) { // исключение возникает, при потери соединения
                System.err.println("Couldn't get I/O for the connection to " + hostName);
            }
        }
    }
}
