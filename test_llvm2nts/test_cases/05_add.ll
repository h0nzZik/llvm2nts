define void @addu() {
  %p = alloca i32, align 4
  store i32 7, i32* %p, align 4  
  %v = load i32* %p
  %sum = add i32 1, %v
  ret void
}
