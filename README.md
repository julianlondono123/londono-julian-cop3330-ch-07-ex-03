# londono-julian-cop3330-ch-07-ex-03

This program implements a basic expression calculator.
          Input from cin; output to cout.
          The grammar for input is:
          Statement:
                    Expression
                    Print
                    Quit
          Print:
                    ;
          Quit:
                    q
          Expression:
                    Term
                    Expression + Term
                    Expression – Term
          Term:
                    Primary
                    Term * Primary
                    Term / Primary
                    Term % Primary
                    Primary !
          Primary:
                    Number
                    ( Expression )
                    – Primary
                    + Primary
                    sqrt (Primary)
          Number:
                    floating-point-literal
