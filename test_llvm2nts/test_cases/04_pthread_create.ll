%union.pthread_attr_t = type { i64, [48 x i8] }

define i8* @thread_func ( i8* %data ) {
	ret i8* null;
}

define void @calee () {
	call i32 @pthread_create (
			i64*                   null,
			%union.pthread_attr_t* null,
			i8* (i8*)*             @thread_func,
			i8*                    null );

	ret void;
}

; Function Attrs: nounwind
declare i32 @pthread_create(i64*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #1

