; ModuleID = 'test2.bc'
source_filename = "iwbasic_module"

@fmt = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str = private unnamed_addr constant [14 x i8] c"Hello, World!\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0))
  %x = alloca i32, align 4
  store i32 42, i32* %x, align 4
  %load = load i32, i32* %x, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fmt.1, i32 0, i32 0), i32 %load)
  ret i32 0
}

declare i32 @printf(i8*, ...)
