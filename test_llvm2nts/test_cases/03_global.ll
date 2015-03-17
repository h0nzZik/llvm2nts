; Global variables. Uses load/store
@G = global i32 17;

define void @tester () {
	%v1 = load i32* @G;
	store i32 3, i32* @G;
	ret void;
}

