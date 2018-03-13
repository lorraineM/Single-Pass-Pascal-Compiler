program simple1;

var a, b, c : integer;
    n : boolean;

FunCtion func2 () : Boolean;
Begin
    func2 := true;
End;

FunCtion soma (a : integer; b : integer) : integer;
Begin
    soma := a + b;
End;

Procedure proc2 ();
var z : char;
Begin
    z := 'Y';
End;

Begin
    n := false;

    if n then
        a := soma(17, 3) * 70;

    if n = true then
        c := (30 * a + 9 / 3) * n;
    else 
        d := func2();
End.
