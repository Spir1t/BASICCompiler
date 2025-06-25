PRINT "Number race"
LET x = 50
LET y = 200000
WHILE x > 5 AND y > 5
REPEAT x = x - 1 y = y / 2 
ENDWHILE
IF y < 5 THEN PRINT "y won! y is "; y; ELSE PRINT "x won! x is "; x;