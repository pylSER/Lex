#int
#if
#while
#void
#else

%(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|0|1|2|3|4|5|6|7|8|9)*
!ID

%({|}|[|])
!Bracket

%(+|-)
!BasicOP

%(<|>)
!Comparison

%(0|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*
!Number

%(;)
!SEMI

%(//(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|0|1|2|3|4|5|6|7|8|9| )*//)
!Comment

%(=)
!ASSIGN

%(!=)
!NEQ

%( )
!SPACE
