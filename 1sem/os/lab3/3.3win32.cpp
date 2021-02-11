//
//  main.cpp
//  lab3_3_WIN32
//
//  Created by Anastasia Taran on 24.12.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

using System;
using System.IO;
using System.Linq;

namespace dir_wind
{
    static class StringExtensions
    {
        public static string TruncateLongString(this string inputString, int maxChars, string postfix = "...") // сокращаем длинную строку
        {
            if (inputString == null || inputString.Length < maxChars) //Ели строка уже короче заданного - возвращаем без изменений
                return inputString;
            maxChars -= postfix.Length;
            var truncatedString = inputString.Substring(0, maxChars) + postfix; //Если нет - сокращаем
            return truncatedString;
        }
    }
    class Manager
    {
        public string CurrentPath { get; set; } = @"C:\";
        private readonly DirCommand dir;
        public Manager(string path = "")
        {
            CurrentPath = path;
            dir = new DirCommand(this); // иничиилизируем объект
        }
        public void Start()
        {
            dir.Handle(); //Читаем файлы с переданной директории
        }
    }
    class DirCommand
    {
        private Manager manager;
        public DirCommand(Manager manager)
        {
            this.manager = manager;
        }
        public void Handle()
        {
            try
            {
                PrintInfo(manager.CurrentPath); // Читаем файлы с переданной директории
            }
            catch //если эксепшн - пшем ошибку
            {
                NotExistsPath(manager.CurrentPath);
            }
        }
        private void PrintInfo(string path)
        {
            var dir = new DirectoryInfo(path);
            FileInfo[] files = dir.GetFiles(); //получаем файлы и диектории
            DirectoryInfo[] directories = dir.GetDirectories();
            Console.WriteLine($"\tPath {path}:");  //печатаем инфу
            Console.WriteLine($"\t\t File #: {files.Length};");
            Console.WriteLine($"\t\t Folders #: {directories.Length};");

            Console.WriteLine($"|{"Type", 6}|{"Name",15}|{"Size",12}|{"Last change",29}|{"Attributes",41}|"); //печатаем табличку
            foreach (DirectoryInfo directory in directories)
            {
                Console.WriteLine($"|{"Folder",6}|{directory.Name.TruncateLongString(15),15}|{"",12}|{directory.LastWriteTime,29}|{directory.Attributes.ToString().TruncateLongString(41),41}|");
            }
            foreach (FileInfo file in files)
            {
                Console.WriteLine($"|{"File", 6}|{file.Name.TruncateLongString(15),15}|{file.Length,12}|{file.LastWriteTime,29}|{file.Attributes.ToString().TruncateLongString(41),41}|");
            }
        }
        public void NotExistsPath(string path)
        {
            Console.ForegroundColor = ConsoleColor.Red; // пишем ошибку КрАсИвО (красненьким)
            Console.WriteLine($"dir ERROR: {path} not exists.");
            Console.ForegroundColor = ConsoleColor.White;
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Manager manager = new Manager("C:/");
            manager.Start();
            Console.ReadKey(); //Ждем нажатия клавиши
        }
    }
}
