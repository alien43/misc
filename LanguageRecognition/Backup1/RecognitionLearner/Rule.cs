using System;
using System.Collections.Generic;

public class Rule
    {
        public string From { get; set; }
        public char To { get; set; }

        public Rule()
        {
        }
        public Rule(string from, char to)
        {
            From = from;
            To = to;
        }

        public class RuleComparer : IEqualityComparer<Rule>
        {
            public bool Equals(Rule lhs, Rule rhs)
            {
                return (lhs.From == rhs.From) && (lhs.To == rhs.To);
            }

            public int GetHashCode(Rule rule)
            {
                return rule.From.GetHashCode() ^ rule.To.GetHashCode();
            }
        }
    }
