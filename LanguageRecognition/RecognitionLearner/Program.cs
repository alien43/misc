using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace RecognitionLearner
{
    class Program
    {
        static void Main(string[] args)
        {
            const uint contextLength = 2;
            string textDirectoryName = "../../../data";
            string[] languages = Directory.GetDirectories(textDirectoryName);
            Console.WriteLine("Started learning with context length " + contextLength.ToString());
            foreach (string languagePath in languages)
            {
                string[] textsFileNames = Directory.GetFiles(languagePath, "*.txt");
                string language = languagePath.Split('/','\\').Last();
                Console.WriteLine("Started processing language " + language);
                Learner learner = new Learner(language, contextLength);
                foreach (string textFileName in textsFileNames)
                {
                    Console.WriteLine("  Parsing file " + textFileName + "...");
                    learner.ParseFile(textFileName);
                }
                string resultsFileName = languagePath + "/" + language + ".lng";
                Console.WriteLine("Writing results to " + language + ".lng");
                learner.WriteResults(resultsFileName);
                Console.WriteLine();
            }
        }
    }
}
