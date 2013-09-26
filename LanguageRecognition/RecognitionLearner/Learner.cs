using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace RecognitionLearner
{
    class Learner
    {
        const uint version = 1;
        
        private string language_;
        private uint contextLength_;
        private Dictionary<Rule, uint> ruleStats_;
        private Dictionary<string, uint> contextStats_;

        public Learner(string language, uint contextLength)
        {
            language_ = language;
            ruleStats_ = new Dictionary<Rule, uint>(new Rule.RuleComparer());
            contextStats_ = new Dictionary<string, uint>();
            contextLength_ = contextLength;
        }

        public void ParseFile(string fileName)
        {
            StreamReader reader;
            try
            {
                reader = new StreamReader(fileName);
            }
            catch
            {
                Console.Error.WriteLine("Could not open text in file ", fileName);
                return;
            }

            string text = "";
            while (!reader.EndOfStream)
            {
                text += reader.ReadLine().ToLower() + ' ';
            }
            reader.Close();

            string context = "";
            foreach (char c in text)
            {
                for (int i = 0; i <= context.Length; ++i)
                {
                    string stringFrom = context.Substring(context.Length - i, i);
                    Rule rule = new Rule(stringFrom, c);
                    if (!ruleStats_.ContainsKey(rule)) {
                        ruleStats_.Add(rule, 0);
                    }
                    ++ruleStats_[rule];
                    if (!contextStats_.ContainsKey(stringFrom))
                    {
                        contextStats_.Add(stringFrom, 0);
                    }
                    ++contextStats_[stringFrom];
                }
                context += c;
                if (context.Length > contextLength_)
                {
                    context = context.Remove(0, 1);
                }
            }
        }

        class RuleStatFilter
        {
            const uint MIN_STRING_COUNT = 500;
            const double MIN_STRING_FREQUENCY = 0.01;

            private uint stringsTotal_;
            private Dictionary<string, uint> contextStats_;

            public RuleStatFilter(uint stringsTotal, Dictionary<string, uint> contextStats)
            {
                stringsTotal_ = stringsTotal;
                contextStats_ = contextStats;
            }

            public bool Filter(KeyValuePair<Rule, uint> kvp)
            {
                string from = kvp.Key.From;
                if (from == "")
                {
                    return false;
                }
                char to = kvp.Key.To;
                uint stringCount = contextStats_[from];
                double stringFrequency = (double)stringCount / stringsTotal_;
                if (stringCount < System.Math.Pow(MIN_STRING_COUNT, 1.0 / from.Length)
                 || stringFrequency < System.Math.Pow(MIN_STRING_FREQUENCY, from.Length))
                {
                    return false;
                }
                return true;
            }
        }

        public void WriteResults(string fileName)
        {
            FileStream fout;
            try
            {
                fout = new FileStream(fileName, FileMode.Create, FileAccess.Write);
            }
            catch
            {
                Console.Error.WriteLine("Could not open result file ", fileName);
                return;
            }

            BinaryWriter writer = new BinaryWriter(fout);
            writer.Write(version);

            
            uint charsTotal = contextStats_[""];
            RuleStatFilter filter = new RuleStatFilter(charsTotal, contextStats_);
            Func<KeyValuePair<Rule, uint>, bool> funcFilter = filter.Filter;
            List<KeyValuePair<Rule, uint>> filteredRuleStats =
                new List<KeyValuePair<Rule, uint>>(ruleStats_.Where(funcFilter));
               

            writer.Write(filteredRuleStats.Count);
            foreach (KeyValuePair<Rule, uint> kvp in filteredRuleStats)
            {
                double ruleFrequency = (double)kvp.Value / contextStats_[kvp.Key.From];
                writer.Write(kvp.Key.From);
                writer.Write(kvp.Key.To);
                writer.Write(ruleFrequency);
            }
            
            writer.Close();
        }
    }
}
