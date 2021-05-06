import java.net.*;
import java.io.*;
import java.util.*;

class Server { // сервер
    ServerSocket server = null; // сервер сокет
    Socket client = null; // клиентский сокет

    public static void main(String[] arg) {
        Server s = new Server();
        s.doConnections(); // вызываем функцию соединения с сервером
    }

    public void doConnections() { // функция соединения
        try{
            server = new ServerSocket(11000);  // запрос на соединение

            while(true) {
                client = server.accept();  // переводим сервер в режим ожидания и ждем подключения клиента
                System.out.println("New client " + client.getRemoteSocketAddress()); // клиент подключился, выводим информацию о клиенте
                ClientThread ct = new ClientThread(client); // под каждого клиента свой поток
                ct.start(); // запуск run()
            }
        }
        catch(Exception e) {
            e.printStackTrace(); // печатаем места, где произошло исключение в исходном коде
        }
    }
}

class ClientThread extends Thread { // поток клиента
    public Socket client = null;
    public PrintWriter out = null;
    public BufferedReader in = null;

    public ClientThread(Socket c) {
        try {
        client = c; // передаем в новый поток сокет клиента
                    // и получаем его потоки записи и чтения
        out = new PrintWriter(client.getOutputStream(), true);  // запись
        in = new BufferedReader(new InputStreamReader(client.getInputStream()));  // чтение
        }
        catch(Exception e) {
            e.printStackTrace(); // печатаем места, где произошло исключение в исходном коде
        }
    }

    public void run() {
        try {
            String inputLine; // наша строка (вводим в командной строке)

            while ((inputLine = in.readLine()) != null) { // если нам есть что считывать
                try {
                    var command = inputLine.split(" ")[0]; // получаем команду

                    switch (command) { // команды
                        default:
                            out.print("No such command \"" + inputLine + "\" ");

                        case "help":
                            out.println("Commands: max, min, mode, median (syntax: command <json array>)");
                            break;

                        case "max":
                            var max = Arrays.stream(parseString(inputLine)).max().orElseThrow(); // находим максимальное значение
                            out.println("Max: " + max);
                            break;

                        case "min":
                            var min = Arrays.stream(parseString(inputLine)).min().orElseThrow(); // находим минимальное значение
                            out.println("Min: " + min);
                            break;

                        case "mode": // значение моды
                            HashMap<Integer, Integer> counter = new HashMap<>(); // создаем хэш таблицу
                            Arrays.stream(parseString(inputLine))
                                    .forEach(arrayElement -> //  пытаемся получить значение каждого элемента
                                            // если такого ключа еще не было -> добавляем и записываем в его значение 1
                                            // если такой ключ уже есть -> увеличиваем значение 1
                                            counter.compute(arrayElement, (k, v) -> (v != null) ? v + 1 : 1));

                            var mode = counter
                                    .entrySet().stream() // возвращаем набор ключ-значение
                                    .max(Comparator.comparingInt(Map.Entry::getValue)) // находим максимальное значение (в нашем случае - максимальное кол-во повторений элемента)
                                    .orElseThrow().getKey(); // возвращаем ключ, что соответствует значению (в нашем случе - число(элемент), которое имеет максимальное ко-во повторений)
                            out.println("Mode: " + mode);
                            break;

                        case "median": // значение медианы
                            var parStr = parseString(inputLine); // парсинг строки
                            var median = Arrays.stream(parStr).sorted().toArray()[parStr.length / 2]; //  сортируем и делим пополам
                            out.println("Median: " + median); // центр - и есть наша медиана
                            break;
                    }
                }
                catch(Exception ignored) {
                    out.println("Wrong syntax (syntax: command <json array>)");
                }
            }
        }
        catch (IOException e) {
            e.printStackTrace(); // печатаем места, где произошло исключение в исходном коде
        }
    }

    private static int[] parseString(String str) {
        var strArr = str.split(" "); // делим по пробелу
        var strNums = strArr[1].substring(1, strArr[1].length() - 1).split(", ?");  // получаем строки цифр
        return Arrays.stream(strNums).mapToInt(Integer::parseInt).toArray(); // преобразовуем каждый элемент стрима в целое число
    }
}