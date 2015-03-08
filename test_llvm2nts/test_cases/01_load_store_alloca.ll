define i32 @test_load_store_alloca () {
	%ptr = alloca i32, align 4
	store i32 15, i32* %ptr
	%value = load i32* %ptr
	ret i32 0
}

; TODO: Test void function
