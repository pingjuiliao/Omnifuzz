; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -mcpu=skx -S -loop-vectorize -instcombine -simplifycfg -licm -force-vector-width=4 -force-vector-interleave=1 -enable-interleaved-mem-accesses -prefer-predicate-over-epilogue=predicate-dont-vectorize < %s | FileCheck %s -check-prefix=DISABLED_MASKED_STRIDED
; RUN: opt -mcpu=skx -S -loop-vectorize -instcombine -simplifycfg -licm -force-vector-width=4 -force-vector-interleave=1 -enable-interleaved-mem-accesses  -enable-masked-interleaved-mem-accesses -prefer-predicate-over-epilogue=predicate-dont-vectorize < %s | FileCheck %s -check-prefix=ENABLED_MASKED_STRIDED

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; (1) Interleave-group with factor 4, storing only 2 members out of the 4.
; Check that when we allow masked-memops to support interleave-group with gaps,
; the store is vectorized using a wide masked store, with a 1,1,0,0,1,1,0,0,... mask.
; Check that when we don't allow masked-memops to support interleave-group with gaps,
; the store is scalarized.
; The input IR was generated from this source:
;     for(i=0;i<1024;i++){
;       points[i*4] = x[i];
;       points[i*4 + 1] = y[i];
;     }
; (relates to the testcase in PR50566)

; Function Attrs: nofree norecurse nosync nounwind uwtable
define dso_local void @test1(i16* noalias nocapture %points, i16* noalias nocapture readonly %x, i16* noalias nocapture readonly %y) local_unnamed_addr {
; DISABLED_MASKED_STRIDED-LABEL: @test1(
; DISABLED_MASKED_STRIDED-NEXT:  entry:
; DISABLED_MASKED_STRIDED-NEXT:    br label [[VECTOR_BODY:%.*]]
; DISABLED_MASKED_STRIDED:       vector.body:
; DISABLED_MASKED_STRIDED-NEXT:    [[INDEX:%.*]] = phi i64 [ 0, [[ENTRY:%.*]] ], [ [[INDEX_NEXT:%.*]], [[VECTOR_BODY]] ]
; DISABLED_MASKED_STRIDED-NEXT:    [[VEC_IND:%.*]] = phi <4 x i64> [ <i64 0, i64 1, i64 2, i64 3>, [[ENTRY]] ], [ [[VEC_IND_NEXT:%.*]], [[VECTOR_BODY]] ]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP0:%.*]] = getelementptr inbounds i16, i16* [[X:%.*]], i64 [[INDEX]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP1:%.*]] = bitcast i16* [[TMP0]] to <4 x i16>*
; DISABLED_MASKED_STRIDED-NEXT:    [[WIDE_LOAD:%.*]] = load <4 x i16>, <4 x i16>* [[TMP1]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP2:%.*]] = shl nuw nsw <4 x i64> [[VEC_IND]], <i64 2, i64 2, i64 2, i64 2>
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP3:%.*]] = extractelement <4 x i64> [[TMP2]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP4:%.*]] = getelementptr inbounds i16, i16* [[POINTS:%.*]], i64 [[TMP3]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP5:%.*]] = extractelement <4 x i64> [[TMP2]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP6:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP5]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP7:%.*]] = extractelement <4 x i64> [[TMP2]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP8:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP7]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP9:%.*]] = extractelement <4 x i64> [[TMP2]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP10:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP9]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP11:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP11]], i16* [[TMP4]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP12:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP12]], i16* [[TMP6]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP13:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP13]], i16* [[TMP8]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP14:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP14]], i16* [[TMP10]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP15:%.*]] = getelementptr inbounds i16, i16* [[Y:%.*]], i64 [[INDEX]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP16:%.*]] = bitcast i16* [[TMP15]] to <4 x i16>*
; DISABLED_MASKED_STRIDED-NEXT:    [[WIDE_LOAD1:%.*]] = load <4 x i16>, <4 x i16>* [[TMP16]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP17:%.*]] = or <4 x i64> [[TMP2]], <i64 1, i64 1, i64 1, i64 1>
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP18:%.*]] = extractelement <4 x i64> [[TMP17]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP19:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP18]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP20:%.*]] = extractelement <4 x i64> [[TMP17]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP21:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP20]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP22:%.*]] = extractelement <4 x i64> [[TMP17]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP23:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP22]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP24:%.*]] = extractelement <4 x i64> [[TMP17]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP25:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP24]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP26:%.*]] = extractelement <4 x i16> [[WIDE_LOAD1]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP26]], i16* [[TMP19]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP27:%.*]] = extractelement <4 x i16> [[WIDE_LOAD1]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP27]], i16* [[TMP21]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP28:%.*]] = extractelement <4 x i16> [[WIDE_LOAD1]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP28]], i16* [[TMP23]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP29:%.*]] = extractelement <4 x i16> [[WIDE_LOAD1]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP29]], i16* [[TMP25]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[INDEX_NEXT]] = add nuw i64 [[INDEX]], 4
; DISABLED_MASKED_STRIDED-NEXT:    [[VEC_IND_NEXT]] = add <4 x i64> [[VEC_IND]], <i64 4, i64 4, i64 4, i64 4>
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP30:%.*]] = icmp eq i64 [[INDEX_NEXT]], 1024
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP30]], label [[FOR_END:%.*]], label [[VECTOR_BODY]], !llvm.loop [[LOOP0:![0-9]+]]
; DISABLED_MASKED_STRIDED:       for.end:
; DISABLED_MASKED_STRIDED-NEXT:    ret void
;
; ENABLED_MASKED_STRIDED-LABEL: @test1(
; ENABLED_MASKED_STRIDED-NEXT:  entry:
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP0:%.*]] = getelementptr inbounds i16, i16* [[POINTS:%.*]], i64 -1
; ENABLED_MASKED_STRIDED-NEXT:    br label [[VECTOR_BODY:%.*]]
; ENABLED_MASKED_STRIDED:       vector.body:
; ENABLED_MASKED_STRIDED-NEXT:    [[INDEX:%.*]] = phi i64 [ 0, [[ENTRY:%.*]] ], [ [[INDEX_NEXT:%.*]], [[VECTOR_BODY]] ]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP1:%.*]] = getelementptr inbounds i16, i16* [[X:%.*]], i64 [[INDEX]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP2:%.*]] = bitcast i16* [[TMP1]] to <4 x i16>*
; ENABLED_MASKED_STRIDED-NEXT:    [[WIDE_LOAD:%.*]] = load <4 x i16>, <4 x i16>* [[TMP2]], align 2
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP3:%.*]] = shl nuw nsw i64 [[INDEX]], 2
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP4:%.*]] = getelementptr inbounds i16, i16* [[Y:%.*]], i64 [[INDEX]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP5:%.*]] = bitcast i16* [[TMP4]] to <4 x i16>*
; ENABLED_MASKED_STRIDED-NEXT:    [[WIDE_LOAD1:%.*]] = load <4 x i16>, <4 x i16>* [[TMP5]], align 2
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP6:%.*]] = or i64 [[TMP3]], 1
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP7:%.*]] = getelementptr inbounds i16, i16* [[TMP0]], i64 [[TMP6]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP8:%.*]] = bitcast i16* [[TMP7]] to <16 x i16>*
; ENABLED_MASKED_STRIDED-NEXT:    [[INTERLEAVED_VEC:%.*]] = shufflevector <4 x i16> [[WIDE_LOAD]], <4 x i16> [[WIDE_LOAD1]], <16 x i32> <i32 0, i32 4, i32 undef, i32 undef, i32 1, i32 5, i32 undef, i32 undef, i32 2, i32 6, i32 undef, i32 undef, i32 3, i32 7, i32 undef, i32 undef>
; ENABLED_MASKED_STRIDED-NEXT:    call void @llvm.masked.store.v16i16.p0v16i16(<16 x i16> [[INTERLEAVED_VEC]], <16 x i16>* [[TMP8]], i32 2, <16 x i1> <i1 true, i1 true, i1 false, i1 false, i1 true, i1 true, i1 false, i1 false, i1 true, i1 true, i1 false, i1 false, i1 true, i1 true, i1 false, i1 false>)
; ENABLED_MASKED_STRIDED-NEXT:    [[INDEX_NEXT]] = add nuw i64 [[INDEX]], 4
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP9:%.*]] = icmp eq i64 [[INDEX_NEXT]], 1024
; ENABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP9]], label [[FOR_END:%.*]], label [[VECTOR_BODY]], !llvm.loop [[LOOP0:![0-9]+]]
; ENABLED_MASKED_STRIDED:       for.end:
; ENABLED_MASKED_STRIDED-NEXT:    ret void
;
entry:
  br label %for.body

for.body:
  %indvars.iv = phi i64 [ 0, %entry ], [ %indvars.iv.next, %for.body ]
  %arrayidx = getelementptr inbounds i16, i16* %x, i64 %indvars.iv
  %0 = load i16, i16* %arrayidx, align 2
  %1 = shl nuw nsw i64 %indvars.iv, 2
  %arrayidx2 = getelementptr inbounds i16, i16* %points, i64 %1
  store i16 %0, i16* %arrayidx2, align 2
  %arrayidx4 = getelementptr inbounds i16, i16* %y, i64 %indvars.iv
  %2 = load i16, i16* %arrayidx4, align 2
  %3 = or i64 %1, 1
  %arrayidx7 = getelementptr inbounds i16, i16* %points, i64 %3
  store i16 %2, i16* %arrayidx7, align 2
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 1024
  br i1 %exitcond.not, label %for.end, label %for.body

for.end:
  ret void
}

; (2) Same as above, but this time the gaps mask of the store is also And-ed with the
; fold-tail mask. If using masked memops to vectorize interleaved-group with gaps is
; not allowed, the store is scalarized and predicated.
; The input IR was generated from this source:
;     for(i=0;i<numPoints;i++){
;       points[i*4] = x[i];
;       points[i*4 + 1] = y[i];
;     }

; Function Attrs: nofree norecurse nosync nounwind uwtable
define dso_local void @test2(i16* noalias nocapture %points, i32 %numPoints, i16* noalias nocapture readonly %x, i16* noalias nocapture readonly %y) local_unnamed_addr {
; DISABLED_MASKED_STRIDED-LABEL: @test2(
; DISABLED_MASKED_STRIDED-NEXT:  entry:
; DISABLED_MASKED_STRIDED-NEXT:    [[CMP15:%.*]] = icmp sgt i32 [[NUMPOINTS:%.*]], 0
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[CMP15]], label [[FOR_BODY_PREHEADER:%.*]], label [[FOR_END:%.*]]
; DISABLED_MASKED_STRIDED:       for.body.preheader:
; DISABLED_MASKED_STRIDED-NEXT:    [[WIDE_TRIP_COUNT:%.*]] = zext i32 [[NUMPOINTS]] to i64
; DISABLED_MASKED_STRIDED-NEXT:    [[N_RND_UP:%.*]] = add nuw nsw i64 [[WIDE_TRIP_COUNT]], 3
; DISABLED_MASKED_STRIDED-NEXT:    [[N_VEC:%.*]] = and i64 [[N_RND_UP]], 8589934588
; DISABLED_MASKED_STRIDED-NEXT:    [[TRIP_COUNT_MINUS_1:%.*]] = add nsw i64 [[WIDE_TRIP_COUNT]], -1
; DISABLED_MASKED_STRIDED-NEXT:    [[BROADCAST_SPLATINSERT:%.*]] = insertelement <4 x i64> poison, i64 [[TRIP_COUNT_MINUS_1]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    [[BROADCAST_SPLAT:%.*]] = shufflevector <4 x i64> [[BROADCAST_SPLATINSERT]], <4 x i64> poison, <4 x i32> zeroinitializer
; DISABLED_MASKED_STRIDED-NEXT:    br label [[VECTOR_BODY:%.*]]
; DISABLED_MASKED_STRIDED:       vector.body:
; DISABLED_MASKED_STRIDED-NEXT:    [[INDEX:%.*]] = phi i64 [ 0, [[FOR_BODY_PREHEADER]] ], [ [[INDEX_NEXT:%.*]], [[PRED_STORE_CONTINUE15:%.*]] ]
; DISABLED_MASKED_STRIDED-NEXT:    [[VEC_IND:%.*]] = phi <4 x i64> [ <i64 0, i64 1, i64 2, i64 3>, [[FOR_BODY_PREHEADER]] ], [ [[VEC_IND_NEXT:%.*]], [[PRED_STORE_CONTINUE15]] ]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP0:%.*]] = icmp ule <4 x i64> [[VEC_IND]], [[BROADCAST_SPLAT]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP1:%.*]] = getelementptr inbounds i16, i16* [[X:%.*]], i64 [[INDEX]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP2:%.*]] = bitcast i16* [[TMP1]] to <4 x i16>*
; DISABLED_MASKED_STRIDED-NEXT:    [[WIDE_MASKED_LOAD:%.*]] = call <4 x i16> @llvm.masked.load.v4i16.p0v4i16(<4 x i16>* [[TMP2]], i32 2, <4 x i1> [[TMP0]], <4 x i16> poison)
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP3:%.*]] = shl nsw <4 x i64> [[VEC_IND]], <i64 2, i64 2, i64 2, i64 2>
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP4:%.*]] = extractelement <4 x i1> [[TMP0]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP4]], label [[PRED_STORE_IF:%.*]], label [[PRED_STORE_CONTINUE:%.*]]
; DISABLED_MASKED_STRIDED:       pred.store.if:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP5:%.*]] = extractelement <4 x i64> [[TMP3]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP6:%.*]] = getelementptr inbounds i16, i16* [[POINTS:%.*]], i64 [[TMP5]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP7:%.*]] = extractelement <4 x i16> [[WIDE_MASKED_LOAD]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP7]], i16* [[TMP6]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE]]
; DISABLED_MASKED_STRIDED:       pred.store.continue:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP8:%.*]] = extractelement <4 x i1> [[TMP0]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP8]], label [[PRED_STORE_IF1:%.*]], label [[PRED_STORE_CONTINUE2:%.*]]
; DISABLED_MASKED_STRIDED:       pred.store.if1:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP9:%.*]] = extractelement <4 x i64> [[TMP3]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP10:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP9]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP11:%.*]] = extractelement <4 x i16> [[WIDE_MASKED_LOAD]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP11]], i16* [[TMP10]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE2]]
; DISABLED_MASKED_STRIDED:       pred.store.continue2:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP12:%.*]] = extractelement <4 x i1> [[TMP0]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP12]], label [[PRED_STORE_IF3:%.*]], label [[PRED_STORE_CONTINUE4:%.*]]
; DISABLED_MASKED_STRIDED:       pred.store.if3:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP13:%.*]] = extractelement <4 x i64> [[TMP3]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP14:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP13]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP15:%.*]] = extractelement <4 x i16> [[WIDE_MASKED_LOAD]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP15]], i16* [[TMP14]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE4]]
; DISABLED_MASKED_STRIDED:       pred.store.continue4:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP16:%.*]] = extractelement <4 x i1> [[TMP0]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP16]], label [[PRED_STORE_IF5:%.*]], label [[PRED_STORE_CONTINUE6:%.*]]
; DISABLED_MASKED_STRIDED:       pred.store.if5:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP17:%.*]] = extractelement <4 x i64> [[TMP3]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP18:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP17]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP19:%.*]] = extractelement <4 x i16> [[WIDE_MASKED_LOAD]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP19]], i16* [[TMP18]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE6]]
; DISABLED_MASKED_STRIDED:       pred.store.continue6:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP20:%.*]] = getelementptr inbounds i16, i16* [[Y:%.*]], i64 [[INDEX]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP21:%.*]] = bitcast i16* [[TMP20]] to <4 x i16>*
; DISABLED_MASKED_STRIDED-NEXT:    [[WIDE_MASKED_LOAD7:%.*]] = call <4 x i16> @llvm.masked.load.v4i16.p0v4i16(<4 x i16>* [[TMP21]], i32 2, <4 x i1> [[TMP0]], <4 x i16> poison)
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP22:%.*]] = or <4 x i64> [[TMP3]], <i64 1, i64 1, i64 1, i64 1>
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP23:%.*]] = extractelement <4 x i1> [[TMP0]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP23]], label [[PRED_STORE_IF8:%.*]], label [[PRED_STORE_CONTINUE9:%.*]]
; DISABLED_MASKED_STRIDED:       pred.store.if8:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP24:%.*]] = extractelement <4 x i64> [[TMP22]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP25:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP24]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP26:%.*]] = extractelement <4 x i16> [[WIDE_MASKED_LOAD7]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP26]], i16* [[TMP25]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE9]]
; DISABLED_MASKED_STRIDED:       pred.store.continue9:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP27:%.*]] = extractelement <4 x i1> [[TMP0]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP27]], label [[PRED_STORE_IF10:%.*]], label [[PRED_STORE_CONTINUE11:%.*]]
; DISABLED_MASKED_STRIDED:       pred.store.if10:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP28:%.*]] = extractelement <4 x i64> [[TMP22]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP29:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP28]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP30:%.*]] = extractelement <4 x i16> [[WIDE_MASKED_LOAD7]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP30]], i16* [[TMP29]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE11]]
; DISABLED_MASKED_STRIDED:       pred.store.continue11:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP31:%.*]] = extractelement <4 x i1> [[TMP0]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP31]], label [[PRED_STORE_IF12:%.*]], label [[PRED_STORE_CONTINUE13:%.*]]
; DISABLED_MASKED_STRIDED:       pred.store.if12:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP32:%.*]] = extractelement <4 x i64> [[TMP22]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP33:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP32]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP34:%.*]] = extractelement <4 x i16> [[WIDE_MASKED_LOAD7]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP34]], i16* [[TMP33]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE13]]
; DISABLED_MASKED_STRIDED:       pred.store.continue13:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP35:%.*]] = extractelement <4 x i1> [[TMP0]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP35]], label [[PRED_STORE_IF14:%.*]], label [[PRED_STORE_CONTINUE15]]
; DISABLED_MASKED_STRIDED:       pred.store.if14:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP36:%.*]] = extractelement <4 x i64> [[TMP22]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP37:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP36]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP38:%.*]] = extractelement <4 x i16> [[WIDE_MASKED_LOAD7]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP38]], i16* [[TMP37]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE15]]
; DISABLED_MASKED_STRIDED:       pred.store.continue15:
; DISABLED_MASKED_STRIDED-NEXT:    [[INDEX_NEXT]] = add i64 [[INDEX]], 4
; DISABLED_MASKED_STRIDED-NEXT:    [[VEC_IND_NEXT]] = add <4 x i64> [[VEC_IND]], <i64 4, i64 4, i64 4, i64 4>
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP39:%.*]] = icmp eq i64 [[INDEX_NEXT]], [[N_VEC]]
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP39]], label [[FOR_END_LOOPEXIT:%.*]], label [[VECTOR_BODY]], !llvm.loop [[LOOP2:![0-9]+]]
; DISABLED_MASKED_STRIDED:       for.end.loopexit:
; DISABLED_MASKED_STRIDED-NEXT:    br label [[FOR_END]]
; DISABLED_MASKED_STRIDED:       for.end:
; DISABLED_MASKED_STRIDED-NEXT:    ret void
;
; ENABLED_MASKED_STRIDED-LABEL: @test2(
; ENABLED_MASKED_STRIDED-NEXT:  entry:
; ENABLED_MASKED_STRIDED-NEXT:    [[CMP15:%.*]] = icmp sgt i32 [[NUMPOINTS:%.*]], 0
; ENABLED_MASKED_STRIDED-NEXT:    br i1 [[CMP15]], label [[FOR_BODY_PREHEADER:%.*]], label [[FOR_END:%.*]]
; ENABLED_MASKED_STRIDED:       for.body.preheader:
; ENABLED_MASKED_STRIDED-NEXT:    [[WIDE_TRIP_COUNT:%.*]] = zext i32 [[NUMPOINTS]] to i64
; ENABLED_MASKED_STRIDED-NEXT:    [[N_RND_UP:%.*]] = add nuw nsw i64 [[WIDE_TRIP_COUNT]], 3
; ENABLED_MASKED_STRIDED-NEXT:    [[N_VEC:%.*]] = and i64 [[N_RND_UP]], 8589934588
; ENABLED_MASKED_STRIDED-NEXT:    [[TRIP_COUNT_MINUS_1:%.*]] = add nsw i64 [[WIDE_TRIP_COUNT]], -1
; ENABLED_MASKED_STRIDED-NEXT:    [[BROADCAST_SPLATINSERT:%.*]] = insertelement <4 x i64> poison, i64 [[TRIP_COUNT_MINUS_1]], i64 0
; ENABLED_MASKED_STRIDED-NEXT:    [[BROADCAST_SPLAT:%.*]] = shufflevector <4 x i64> [[BROADCAST_SPLATINSERT]], <4 x i64> poison, <4 x i32> zeroinitializer
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP0:%.*]] = getelementptr inbounds i16, i16* [[POINTS:%.*]], i64 -1
; ENABLED_MASKED_STRIDED-NEXT:    br label [[VECTOR_BODY:%.*]]
; ENABLED_MASKED_STRIDED:       vector.body:
; ENABLED_MASKED_STRIDED-NEXT:    [[INDEX:%.*]] = phi i64 [ 0, [[FOR_BODY_PREHEADER]] ], [ [[INDEX_NEXT:%.*]], [[VECTOR_BODY]] ]
; ENABLED_MASKED_STRIDED-NEXT:    [[BROADCAST_SPLATINSERT1:%.*]] = insertelement <4 x i64> poison, i64 [[INDEX]], i64 0
; ENABLED_MASKED_STRIDED-NEXT:    [[BROADCAST_SPLAT2:%.*]] = shufflevector <4 x i64> [[BROADCAST_SPLATINSERT1]], <4 x i64> poison, <4 x i32> zeroinitializer
; ENABLED_MASKED_STRIDED-NEXT:    [[INDUCTION:%.*]] = or <4 x i64> [[BROADCAST_SPLAT2]], <i64 0, i64 1, i64 2, i64 3>
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP1:%.*]] = icmp ule <4 x i64> [[INDUCTION]], [[BROADCAST_SPLAT]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP2:%.*]] = getelementptr inbounds i16, i16* [[X:%.*]], i64 [[INDEX]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP3:%.*]] = bitcast i16* [[TMP2]] to <4 x i16>*
; ENABLED_MASKED_STRIDED-NEXT:    [[WIDE_MASKED_LOAD:%.*]] = call <4 x i16> @llvm.masked.load.v4i16.p0v4i16(<4 x i16>* [[TMP3]], i32 2, <4 x i1> [[TMP1]], <4 x i16> poison)
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP4:%.*]] = shl nsw i64 [[INDEX]], 2
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP5:%.*]] = getelementptr inbounds i16, i16* [[Y:%.*]], i64 [[INDEX]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP6:%.*]] = bitcast i16* [[TMP5]] to <4 x i16>*
; ENABLED_MASKED_STRIDED-NEXT:    [[WIDE_MASKED_LOAD3:%.*]] = call <4 x i16> @llvm.masked.load.v4i16.p0v4i16(<4 x i16>* [[TMP6]], i32 2, <4 x i1> [[TMP1]], <4 x i16> poison)
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP7:%.*]] = or i64 [[TMP4]], 1
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP8:%.*]] = getelementptr inbounds i16, i16* [[TMP0]], i64 [[TMP7]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP9:%.*]] = bitcast i16* [[TMP8]] to <16 x i16>*
; ENABLED_MASKED_STRIDED-NEXT:    [[INTERLEAVED_VEC:%.*]] = shufflevector <4 x i16> [[WIDE_MASKED_LOAD]], <4 x i16> [[WIDE_MASKED_LOAD3]], <16 x i32> <i32 0, i32 4, i32 undef, i32 undef, i32 1, i32 5, i32 undef, i32 undef, i32 2, i32 6, i32 undef, i32 undef, i32 3, i32 7, i32 undef, i32 undef>
; ENABLED_MASKED_STRIDED-NEXT:    [[INTERLEAVED_MASK:%.*]] = shufflevector <4 x i1> [[TMP1]], <4 x i1> poison, <16 x i32> <i32 0, i32 0, i32 0, i32 0, i32 1, i32 1, i32 1, i32 1, i32 2, i32 2, i32 2, i32 2, i32 3, i32 3, i32 3, i32 3>
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP10:%.*]] = and <16 x i1> [[INTERLEAVED_MASK]], <i1 true, i1 true, i1 false, i1 false, i1 true, i1 true, i1 false, i1 false, i1 true, i1 true, i1 false, i1 false, i1 true, i1 true, i1 false, i1 false>
; ENABLED_MASKED_STRIDED-NEXT:    call void @llvm.masked.store.v16i16.p0v16i16(<16 x i16> [[INTERLEAVED_VEC]], <16 x i16>* [[TMP9]], i32 2, <16 x i1> [[TMP10]])
; ENABLED_MASKED_STRIDED-NEXT:    [[INDEX_NEXT]] = add i64 [[INDEX]], 4
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP11:%.*]] = icmp eq i64 [[INDEX_NEXT]], [[N_VEC]]
; ENABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP11]], label [[FOR_END_LOOPEXIT:%.*]], label [[VECTOR_BODY]], !llvm.loop [[LOOP2:![0-9]+]]
; ENABLED_MASKED_STRIDED:       for.end.loopexit:
; ENABLED_MASKED_STRIDED-NEXT:    br label [[FOR_END]]
; ENABLED_MASKED_STRIDED:       for.end:
; ENABLED_MASKED_STRIDED-NEXT:    ret void
;
entry:
  %cmp15 = icmp sgt i32 %numPoints, 0
  br i1 %cmp15, label %for.body.preheader, label %for.end

for.body.preheader:
  %wide.trip.count = zext i32 %numPoints to i64
  br label %for.body

for.body:
  %indvars.iv = phi i64 [ 0, %for.body.preheader ], [ %indvars.iv.next, %for.body ]
  %arrayidx = getelementptr inbounds i16, i16* %x, i64 %indvars.iv
  %0 = load i16, i16* %arrayidx, align 2
  %1 = shl nsw i64 %indvars.iv, 2
  %arrayidx2 = getelementptr inbounds i16, i16* %points, i64 %1
  store i16 %0, i16* %arrayidx2, align 2
  %arrayidx4 = getelementptr inbounds i16, i16* %y, i64 %indvars.iv
  %2 = load i16, i16* %arrayidx4, align 2
  %3 = or i64 %1, 1
  %arrayidx7 = getelementptr inbounds i16, i16* %points, i64 %3
  store i16 %2, i16* %arrayidx7, align 2
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, %wide.trip.count
  br i1 %exitcond.not, label %for.end.loopexit, label %for.body

for.end.loopexit:
  br label %for.end

for.end:
  ret void
}

; (3) Testing a scenario of a conditional store. The gaps mask of the store is also
; And-ed with the condition mask (x[i] > 0).
; If using masked memops to vectorize interleaved-group with gaps is
; not allowed, the store is scalarized and predicated.
; Here the Interleave-group is with factor 3, storing only 1 member out of the 3.
; The input IR was generated from this source:
;     for(i=0;i<1024;i++){
;       if (x[i] > 0)
;         points[i*3] = x[i];
;     }
; Function Attrs: nofree norecurse nosync nounwind uwtable
define dso_local void @test(i16* noalias nocapture %points, i16* noalias nocapture readonly %x, i16* noalias nocapture readnone %y) local_unnamed_addr {
; DISABLED_MASKED_STRIDED-LABEL: @test(
; DISABLED_MASKED_STRIDED-NEXT:  entry:
; DISABLED_MASKED_STRIDED-NEXT:    br label [[VECTOR_BODY:%.*]]
; DISABLED_MASKED_STRIDED:       vector.body:
; DISABLED_MASKED_STRIDED-NEXT:    [[INDEX:%.*]] = phi i64 [ 0, [[ENTRY:%.*]] ], [ [[INDEX_NEXT:%.*]], [[PRED_STORE_CONTINUE6:%.*]] ]
; DISABLED_MASKED_STRIDED-NEXT:    [[VEC_IND:%.*]] = phi <4 x i64> [ <i64 0, i64 1, i64 2, i64 3>, [[ENTRY]] ], [ [[VEC_IND_NEXT:%.*]], [[PRED_STORE_CONTINUE6]] ]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP0:%.*]] = getelementptr inbounds i16, i16* [[X:%.*]], i64 [[INDEX]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP1:%.*]] = bitcast i16* [[TMP0]] to <4 x i16>*
; DISABLED_MASKED_STRIDED-NEXT:    [[WIDE_LOAD:%.*]] = load <4 x i16>, <4 x i16>* [[TMP1]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP2:%.*]] = icmp sgt <4 x i16> [[WIDE_LOAD]], zeroinitializer
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP3:%.*]] = mul nuw nsw <4 x i64> [[VEC_IND]], <i64 3, i64 3, i64 3, i64 3>
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP4:%.*]] = extractelement <4 x i1> [[TMP2]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP4]], label [[PRED_STORE_IF:%.*]], label [[PRED_STORE_CONTINUE:%.*]]
; DISABLED_MASKED_STRIDED:       pred.store.if:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP5:%.*]] = extractelement <4 x i64> [[TMP3]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP6:%.*]] = getelementptr inbounds i16, i16* [[POINTS:%.*]], i64 [[TMP5]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP7:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 0
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP7]], i16* [[TMP6]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE]]
; DISABLED_MASKED_STRIDED:       pred.store.continue:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP8:%.*]] = extractelement <4 x i1> [[TMP2]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP8]], label [[PRED_STORE_IF1:%.*]], label [[PRED_STORE_CONTINUE2:%.*]]
; DISABLED_MASKED_STRIDED:       pred.store.if1:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP9:%.*]] = extractelement <4 x i64> [[TMP3]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP10:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP9]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP11:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 1
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP11]], i16* [[TMP10]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE2]]
; DISABLED_MASKED_STRIDED:       pred.store.continue2:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP12:%.*]] = extractelement <4 x i1> [[TMP2]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP12]], label [[PRED_STORE_IF3:%.*]], label [[PRED_STORE_CONTINUE4:%.*]]
; DISABLED_MASKED_STRIDED:       pred.store.if3:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP13:%.*]] = extractelement <4 x i64> [[TMP3]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP14:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP13]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP15:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 2
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP15]], i16* [[TMP14]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE4]]
; DISABLED_MASKED_STRIDED:       pred.store.continue4:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP16:%.*]] = extractelement <4 x i1> [[TMP2]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP16]], label [[PRED_STORE_IF5:%.*]], label [[PRED_STORE_CONTINUE6]]
; DISABLED_MASKED_STRIDED:       pred.store.if5:
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP17:%.*]] = extractelement <4 x i64> [[TMP3]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP18:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP17]]
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP19:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 3
; DISABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP19]], i16* [[TMP18]], align 2
; DISABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE6]]
; DISABLED_MASKED_STRIDED:       pred.store.continue6:
; DISABLED_MASKED_STRIDED-NEXT:    [[INDEX_NEXT]] = add nuw i64 [[INDEX]], 4
; DISABLED_MASKED_STRIDED-NEXT:    [[VEC_IND_NEXT]] = add <4 x i64> [[VEC_IND]], <i64 4, i64 4, i64 4, i64 4>
; DISABLED_MASKED_STRIDED-NEXT:    [[TMP20:%.*]] = icmp eq i64 [[INDEX_NEXT]], 1024
; DISABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP20]], label [[FOR_END:%.*]], label [[VECTOR_BODY]], !llvm.loop [[LOOP3:![0-9]+]]
; DISABLED_MASKED_STRIDED:       for.end:
; DISABLED_MASKED_STRIDED-NEXT:    ret void
;
; ENABLED_MASKED_STRIDED-LABEL: @test(
; ENABLED_MASKED_STRIDED-NEXT:  entry:
; ENABLED_MASKED_STRIDED-NEXT:    br label [[VECTOR_BODY:%.*]]
; ENABLED_MASKED_STRIDED:       vector.body:
; ENABLED_MASKED_STRIDED-NEXT:    [[INDEX:%.*]] = phi i64 [ 0, [[ENTRY:%.*]] ], [ [[INDEX_NEXT:%.*]], [[PRED_STORE_CONTINUE6:%.*]] ]
; ENABLED_MASKED_STRIDED-NEXT:    [[VEC_IND:%.*]] = phi <4 x i64> [ <i64 0, i64 1, i64 2, i64 3>, [[ENTRY]] ], [ [[VEC_IND_NEXT:%.*]], [[PRED_STORE_CONTINUE6]] ]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP0:%.*]] = getelementptr inbounds i16, i16* [[X:%.*]], i64 [[INDEX]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP1:%.*]] = bitcast i16* [[TMP0]] to <4 x i16>*
; ENABLED_MASKED_STRIDED-NEXT:    [[WIDE_LOAD:%.*]] = load <4 x i16>, <4 x i16>* [[TMP1]], align 2
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP2:%.*]] = icmp sgt <4 x i16> [[WIDE_LOAD]], zeroinitializer
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP3:%.*]] = mul nuw nsw <4 x i64> [[VEC_IND]], <i64 3, i64 3, i64 3, i64 3>
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP4:%.*]] = extractelement <4 x i1> [[TMP2]], i64 0
; ENABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP4]], label [[PRED_STORE_IF:%.*]], label [[PRED_STORE_CONTINUE:%.*]]
; ENABLED_MASKED_STRIDED:       pred.store.if:
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP5:%.*]] = extractelement <4 x i64> [[TMP3]], i64 0
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP6:%.*]] = getelementptr inbounds i16, i16* [[POINTS:%.*]], i64 [[TMP5]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP7:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 0
; ENABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP7]], i16* [[TMP6]], align 2
; ENABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE]]
; ENABLED_MASKED_STRIDED:       pred.store.continue:
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP8:%.*]] = extractelement <4 x i1> [[TMP2]], i64 1
; ENABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP8]], label [[PRED_STORE_IF1:%.*]], label [[PRED_STORE_CONTINUE2:%.*]]
; ENABLED_MASKED_STRIDED:       pred.store.if1:
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP9:%.*]] = extractelement <4 x i64> [[TMP3]], i64 1
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP10:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP9]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP11:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 1
; ENABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP11]], i16* [[TMP10]], align 2
; ENABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE2]]
; ENABLED_MASKED_STRIDED:       pred.store.continue2:
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP12:%.*]] = extractelement <4 x i1> [[TMP2]], i64 2
; ENABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP12]], label [[PRED_STORE_IF3:%.*]], label [[PRED_STORE_CONTINUE4:%.*]]
; ENABLED_MASKED_STRIDED:       pred.store.if3:
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP13:%.*]] = extractelement <4 x i64> [[TMP3]], i64 2
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP14:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP13]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP15:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 2
; ENABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP15]], i16* [[TMP14]], align 2
; ENABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE4]]
; ENABLED_MASKED_STRIDED:       pred.store.continue4:
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP16:%.*]] = extractelement <4 x i1> [[TMP2]], i64 3
; ENABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP16]], label [[PRED_STORE_IF5:%.*]], label [[PRED_STORE_CONTINUE6]]
; ENABLED_MASKED_STRIDED:       pred.store.if5:
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP17:%.*]] = extractelement <4 x i64> [[TMP3]], i64 3
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP18:%.*]] = getelementptr inbounds i16, i16* [[POINTS]], i64 [[TMP17]]
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP19:%.*]] = extractelement <4 x i16> [[WIDE_LOAD]], i64 3
; ENABLED_MASKED_STRIDED-NEXT:    store i16 [[TMP19]], i16* [[TMP18]], align 2
; ENABLED_MASKED_STRIDED-NEXT:    br label [[PRED_STORE_CONTINUE6]]
; ENABLED_MASKED_STRIDED:       pred.store.continue6:
; ENABLED_MASKED_STRIDED-NEXT:    [[INDEX_NEXT]] = add nuw i64 [[INDEX]], 4
; ENABLED_MASKED_STRIDED-NEXT:    [[VEC_IND_NEXT]] = add <4 x i64> [[VEC_IND]], <i64 4, i64 4, i64 4, i64 4>
; ENABLED_MASKED_STRIDED-NEXT:    [[TMP20:%.*]] = icmp eq i64 [[INDEX_NEXT]], 1024
; ENABLED_MASKED_STRIDED-NEXT:    br i1 [[TMP20]], label [[FOR_END:%.*]], label [[VECTOR_BODY]], !llvm.loop [[LOOP3:![0-9]+]]
; ENABLED_MASKED_STRIDED:       for.end:
; ENABLED_MASKED_STRIDED-NEXT:    ret void
;
entry:
  br label %for.body

for.body:
  %indvars.iv = phi i64 [ 0, %entry ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds i16, i16* %x, i64 %indvars.iv
  %0 = load i16, i16* %arrayidx, align 2
  %cmp1 = icmp sgt i16 %0, 0
  br i1 %cmp1, label %if.then, label %for.inc

if.then:
  %1 = mul nuw nsw i64 %indvars.iv, 3
  %arrayidx6 = getelementptr inbounds i16, i16* %points, i64 %1
  store i16 %0, i16* %arrayidx6, align 2
  br label %for.inc

for.inc:
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 1024
  br i1 %exitcond.not, label %for.end, label %for.body

for.end:
  ret void
}