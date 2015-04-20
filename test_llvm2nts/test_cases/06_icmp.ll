define void @icmp_test() {
  %p = alloca i32, align 4
  store i32 7, i32* %p, align 4  
  %v = load i32* %p
    
  %fl_1 = icmp ne i32 %v, 10
  %fl_2 = icmp eq i32 %v, 10
  %fl_3 = icmp ugt i32 %v, 10
  %fl_4 = icmp uge i32 %v, 10
  %fl_5 = icmp ule i32 %v, 10
  %fl_6 = icmp ult i32 %v, 10
  
  ret void
}
