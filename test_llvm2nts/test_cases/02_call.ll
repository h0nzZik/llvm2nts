define void @a ( i32 %arg ) {
	ret void;
}

define void @b () {
	ret void;
}

define i32 @c () {
	ret i32 15;
}

define i32 @d ( i32 %arg ) {
	ret i32 %arg;
}

define void @calee () {
	call void @a ( i32 7 );
	call void @b ();
	%v1 = call i32 @c ();
	%v2 = call i32 @d ( i32 %v1 );
	ret void;
}

