using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace QuintaEssentia
{
    class Lang
    {
        private String[] currentLang;
        private Dictionary<String, String[]> langs;
        public static Lang main;
        public Lang()
        {
            langs = new Dictionary<string, string[]>();
            ChangeLang("ENG");
        }
        public static void Create()
        {
            if (main == null)
                main = new Lang();
        }
        private void ChangeLang(string newLang)
        {
            if (langs.ContainsKey(newLang))
            {
                currentLang = langs[newLang];
                return;
            }
            if (File.Exists("Resources\\Langs\\" + newLang + ".txt"))
            {
                StreamReader reader = new StreamReader("Resources\\Langs\\" + newLang + ".txt");
                int n = Convert.ToInt32(reader.ReadLine());
                currentLang = new String[n];
                for (int i = 0; i < n; i++)
                    currentLang[i] = reader.ReadLine().Replace("\\n","\n");
                langs.Add(newLang, currentLang);
                reader.Close();
            }
        }
        public String GetText(int index)
        {
            return currentLang[index];
        }
    }
}
