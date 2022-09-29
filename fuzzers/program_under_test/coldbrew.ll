; ModuleID = 'coldbrew.c'
source_filename = "coldbrew.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque

@stderr = external dso_local local_unnamed_addr global %struct._IO_FILE*, align 8
@.str = private unnamed_addr constant [44 x i8] c"[Error] Program under test failed on read()\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"hello world\00", align 1

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32 noundef %0, i8** nocapture noundef readnone %1) local_unnamed_addr #0 {
  %3 = alloca [20 x i8], align 16
  %4 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i64 0, i64 0
  call void @llvm.lifetime.start.p0i8(i64 20, i8* nonnull %4) #6
  %5 = call i64 @read(i32 noundef 0, i8* noundef nonnull %4, i64 noundef 20) #6
  %6 = trunc i64 %5 to i32
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %8, label %11

8:                                                ; preds = %2
  %9 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !tbaa !3
  %10 = call i64 @fwrite(i8* getelementptr inbounds ([44 x i8], [44 x i8]* @.str, i64 0, i64 0), i64 43, i64 1, %struct._IO_FILE* %9) #7
  call void @exit(i32 noundef -1) #8
  unreachable

11:                                               ; preds = %2
  %12 = load i8, i8* %4, align 16, !tbaa !7
  %13 = icmp eq i8 %12, 99
  %14 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i64 0, i64 1
  %15 = load i8, i8* %14, align 1
  %16 = icmp eq i8 %15, 111
  %17 = select i1 %13, i1 %16, i1 false
  %18 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i64 0, i64 2
  %19 = load i8, i8* %18, align 2
  %20 = icmp eq i8 %19, 108
  %21 = select i1 %17, i1 %20, i1 false
  %22 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i64 0, i64 3
  %23 = load i8, i8* %22, align 1
  %24 = icmp eq i8 %23, 100
  %25 = select i1 %21, i1 %24, i1 false
  %26 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i64 0, i64 4
  %27 = load i8, i8* %26, align 4
  %28 = icmp eq i8 %27, 98
  %29 = select i1 %25, i1 %28, i1 false
  %30 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i64 0, i64 5
  %31 = load i8, i8* %30, align 1
  %32 = icmp eq i8 %31, 114
  %33 = select i1 %29, i1 %32, i1 false
  %34 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i64 0, i64 6
  %35 = load i8, i8* %34, align 2
  %36 = icmp eq i8 %35, 101
  %37 = select i1 %33, i1 %36, i1 false
  %38 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i64 0, i64 7
  %39 = load i8, i8* %38, align 1
  %40 = icmp eq i8 %39, 119
  %41 = select i1 %37, i1 %40, i1 false
  br i1 %41, label %42, label %44

42:                                               ; preds = %11
  %43 = call i32 @puts(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i64 0, i64 0))
  br label %44

44:                                               ; preds = %42, %11
  call void @llvm.lifetime.end.p0i8(i64 20, i8* nonnull %4) #6
  ret i32 0
}

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nofree
declare dso_local noundef i64 @read(i32 noundef, i8* nocapture noundef, i64 noundef) local_unnamed_addr #2

; Function Attrs: noreturn nounwind
declare dso_local void @exit(i32 noundef) local_unnamed_addr #3

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #4

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nofree nounwind
declare noundef i64 @fwrite(i8* nocapture noundef, i64 noundef, i64 noundef, %struct._IO_FILE* nocapture noundef) local_unnamed_addr #5

attributes #0 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly mustprogress nofree nosync nounwind willreturn }
attributes #2 = { nofree "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { noreturn nounwind "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree nounwind "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nofree nounwind }
attributes #6 = { nounwind }
attributes #7 = { cold }
attributes #8 = { noreturn nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{!"clang version 14.0.6 (git@github.com:pingjuiliao/Omnifuzz.git 8540015cf844e17e52a7d41dd6299a6098c40776)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"any pointer", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{!5, !5, i64 0}
