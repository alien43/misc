using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Recognizer
{
    class Program
    {
        static void Main(string[] args)
        {
            string textDirectoryName = "../../../data";
            string[] languages = Directory.GetDirectories(textDirectoryName);
            List<Recognizer> recognizers = new List<Recognizer>();
            foreach (string languagePath in languages)
            {
                string language = languagePath.Split('/', '\\').Last();
                Config recognizerConfig = new Config();
                recognizerConfig.language = language;
                recognizerConfig.consideringType = Config.ConsideringType.SUM;
                recognizerConfig.consideredContextLength = 2;
                Recognizer recognizer = new Recognizer(recognizerConfig);
                string statsFileName = languagePath + "/" + language + ".lng";
                recognizer.LoadStats(statsFileName);
                recognizers.Add(recognizer);
            }

            string inputFileName = textDirectoryName + "/" + "input.txt";
            StreamReader fin;
            try
            {
                fin = new StreamReader(inputFileName);
            }
            catch
            {
                Console.Error.WriteLine("Could not open input file ", inputFileName);
                return;
            }
            
            uint linesCount = uint.Parse(fin.ReadLine());
            for (uint iLine = 0; iLine < linesCount; ++iLine)
            {
                string text = fin.ReadLine();
                double maxLikelihood = double.MinValue;
                string language = "Unknown";
                foreach (Recognizer recognizer in recognizers)
                {
                    string recognizerLang = recognizer.GetLanguage();
                    double likelihood = recognizer.GetLikelihood(text);
                    if (maxLikelihood < likelihood)
                    {
                        maxLikelihood = likelihood;
                        language = recognizerLang;
                    }
                }
                Console.Out.WriteLine(text);
                Console.Out.WriteLine(" - " + language);
            }
        }
    }
}
