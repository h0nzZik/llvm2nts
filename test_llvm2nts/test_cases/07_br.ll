define void @main () {
	br i1 1, label %IfTrue, label %IfFalse
IfTrue:
	br label %IfFalse;
IfFalse:
	ret void;
}

