using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Recognizer
{
    class Config
    {
        public double defaultProbability { get; set; }
        public string language { get; set; }
        public enum ConsideringType { SINGLE, SUM, PRODUCT };
        public ConsideringType consideringType { get; set; }
        public int consideredContextLength { get; set; }

        public Config()
        {
            defaultProbability = 0.0001;
            language = "Unknown language";
            consideringType = ConsideringType.SINGLE;
            consideredContextLength = 1;
        }
    }

    class Recognizer
    {
        private Config config_;
        private Dictionary<Rule, double> ruleStats_;
        private int maxRuleLength_;

        public Recognizer(Config config)
        {
            config_ = config;
            ruleStats_ = new Dictionary<Rule, double>(new Rule.RuleComparer());
            maxRuleLength_ = 0;
        }

        public void LoadStats(string fileName)
        {
            FileStream fin;
            try
            {
                fin = new FileStream(fileName, FileMode.Open, FileAccess.Read);
            }
            catch
            {
                Console.Error.WriteLine("Could not open stats file ", fileName);
                return;
            }
            BinaryReader reader = new BinaryReader(fin);
            UInt32 version = reader.ReadUInt32();
            UInt32 rulesCount = reader.ReadUInt32();
            for (uint i = 0; i < rulesCount; ++i)
            {
                string from = reader.ReadString();
                char to = reader.ReadChar();
                Rule rule = new Rule(from, to);
                double frequency = reader.ReadDouble();
                ruleStats_.Add(rule, frequency);
            }
            reader.Close();

            foreach (Rule rule in ruleStats_.Keys)
            {
                int ruleLength = rule.From.Length;
                if (maxRuleLength_ < ruleLength)
                {
                    maxRuleLength_ = ruleLength;
                }
            }
        }

        public double GetLikelihood(string text)
        {
            if (text.Length == 0)
            {
                return 0.0;
            }

            string processedText = text.ToLower();

            double[] probabilities = new double[maxRuleLength_];
            for (int iStart = 0; iStart < text.Length; ++iStart)
            {
                for (int length = 1;
                    length <= maxRuleLength_ && iStart + length + 1 < text.Length;
                    ++length)
                {
                    string from = processedText.Substring(iStart, length);
                    char to = processedText[iStart + length];
                    Rule rule = new Rule(from, to);
                    double prob = System.Math.Pow(config_.defaultProbability, length);
                    if (ruleStats_.ContainsKey(rule))
                    {
                        prob = ruleStats_[rule];
                    }
                    probabilities[length - 1] += System.Math.Log(prob);
                }
            }
            for (int length = 1; length <= maxRuleLength_; ++length)
            {
                probabilities[length - 1] /= text.Length;
            }
            double answer = 0.0;
            if (config_.consideringType == Config.ConsideringType.SINGLE)
            {
                answer = System.Math.Exp(probabilities[config_.consideredContextLength - 1]);
            }
            else if (config_.consideringType == Config.ConsideringType.SUM)
            {
                answer = 0.0;
                for (int i = 0; i < config_.consideredContextLength; ++i)
                {
                    answer += System.Math.Exp(probabilities[i]);
                }
            }
            else if (config_.consideringType == Config.ConsideringType.PRODUCT)
            {
                double sum = 0.0;
                for (int i = 0; i < config_.consideredContextLength; ++i)
                {
                    sum += probabilities[i];
                }
                answer = System.Math.Exp(sum);
            }
            return answer;
        }

        public string GetLanguage()
        {
            return config_.language;
        }
    }
}
