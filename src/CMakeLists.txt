
ADD_LIBRARY(gosslib
	GossCmdReg.cc App.cc
	#AsyncMerge.cc
	BackyardHash.cc
	Debug.cc
	DenseArray.cc
	BigInteger.cc
	#EntryEdgeSet.cc
	FileFactory.cc
	GossamerException.cc
	GossFastaReadBaseString.cc
	GossFastqReadBaseString.cc
	GossReadBaseString.cc
	Graph.cc
	IntegerArray.cc
	LineSource.cc
	MachDep.cc
	PhysicalFileFactory.cc
	#Profile.cc
	SmallBaseVector.cc
	#SuperGraph.cc
	SparseArray.cc
	StringFileFactory.cc
	Utils.cc
	VariableByteArray.cc
	VariableByteArray.cc
	WordyBitVector.cc
)

TARGET_LINK_LIBRARIES(gosslib
	${Boost_SYSTEM_LIBRARY}
	${Boost_IOSTREAMS_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	${ZLIB_LIBRARIES}
	${BZIP2_LIBRARIES}
	${SQLITE_LIBRARIES}
	Threads::Threads
)


ADD_LIBRARY(gossapp
	GossApp.cc GossOption.cc 
	GossCmdBuildGraph.cc
	#GossCmdLintGraph.cc
	GossCmdPrintContigs.cc
	${Boost_PROGRAM_OPTIONS_LIBRARY}
)


# goss

TARGET_LINK_LIBRARIES(gossapp gosslib ${Boost_PROGRAM_OPTIONS_LIBRARY})

ADD_EXECUTABLE(goss goss.cc)

TARGET_LINK_LIBRARIES(goss gossapp)


# Unit tests

if(BUILD_tests)

MACRO(gossamer_unit_test t src)
   ADD_EXECUTABLE(${t} ${src})
   ADD_TEST(NAME ${t} COMMAND ${t})

   set(extra_macro_args ${ARGN})
   list(LENGTH extra_macro_args num_extra_args)

   set(extra_libs gosslib)

   if (${num_extra_args} GREATER 0)
       list(GET extra_macro_args 0 lib)
       list(APPEND extra_libs ${lib})
   endif ()
   TARGET_LINK_LIBRARIES(${t} ${extra_libs})
ENDMACRO(gossamer_unit_test)

gossamer_unit_test(testAnotTree testAnnotTree.cc)
gossamer_unit_test(testAsyncMerge testAsyncMerge.cc)
gossamer_unit_test(testBackgroundLineSource testBackgroundLineSource.cc)
gossamer_unit_test(testBackyardHash testBackyardHash.cc)
gossamer_unit_test(testBigInteger testBigInteger.cc)
gossamer_unit_test(testBitVecSet testBitVecSet.cc)
gossamer_unit_test(testBlendedSort testBlendedSort.cc)
gossamer_unit_test(testBoundedQueue testBoundedQueue.cc)
gossamer_unit_test(testCompactDynamicBitVector testCompactDynamicBitVector.cc)
gossamer_unit_test(testDenseArray testDenseArray.cc)
gossamer_unit_test(testEdgeAndCount testEdgeAndCount.cc)
gossamer_unit_test(testEnumerativeCode testEnumerativeCode.cc)
gossamer_unit_test(testEstimateGraphStatistics testEstimateGraphStatistics.cc)
gossamer_unit_test(testExternalVarPushSorter testExternalVarPushSorter.cc)
gossamer_unit_test(testExternalBufferSort testExternalBufferSort.cc)
gossamer_unit_test(testFastqParser testFastqParser.cc)
gossamer_unit_test(testFeistelHash testFeistelHash.cc)
gossamer_unit_test(testFibHeap testFibHeap.cc)
gossamer_unit_test(testFixedWidthBitArray testFixedWidthBitArray.cc)
gossamer_unit_test(testGammaCodec testGammaCodec.cc)
gossamer_unit_test(testGossReadBaseString testGossReadBaseString.cc)
gossamer_unit_test(testGossReadSequenceBases testGossReadSequenceBases.cc)
gossamer_unit_test(testGraph testGraph.cc)
gossamer_unit_test(testJobManager testJobManager.cc)
gossamer_unit_test(testKmerAligner testKmerAligner.cc gossapp)
gossamer_unit_test(testKmerIndex testKmerIndex.cc)
gossamer_unit_test(testLevenbergMarquardt testLevenbergMarquardt.cc)
gossamer_unit_test(testLineParser testLineParser.cc)
gossamer_unit_test(testMultithreadedBatchTask testMultithreadedBatchTask.cc)
gossamer_unit_test(testPlainLineSource testPlainLineSource.cc)
gossamer_unit_test(testPhysicalFileFactory testPhysicalFileFactory.cc)
gossamer_unit_test(testRRRArray testRRRArray.cc)
gossamer_unit_test(testReverseComplementAdapter testReverseComplementAdapter.cc)
gossamer_unit_test(testRunLengthCodedBitVectorWord testRunLengthCodedBitVectorWord.cc)
gossamer_unit_test(testRunLengthCodedSet testRunLengthCodedSet.cc)
gossamer_unit_test(testSimpleHashMap testSimpleHashMap.cc)
gossamer_unit_test(testSimpleHashSet testSimpleHashSet.cc)
gossamer_unit_test(testSimpleRangeSet testSimpleRangeSet.cc)
gossamer_unit_test(testSmallBaseVector testSmallBaseVector.cc)
gossamer_unit_test(testSortedArrayMap testSortedArrayMap.cc)
gossamer_unit_test(testSparseArray testSparseArray.cc)
gossamer_unit_test(testSparseArrayView testSparseArrayView.cc)
gossamer_unit_test(testSpinlock testSpinlock.cc)
gossamer_unit_test(testTourBus testTourBus.cc)
gossamer_unit_test(testUtils testUtils.cc)
gossamer_unit_test(testVariableByteArray testVariableByteArray.cc)
gossamer_unit_test(testVariableWidthBitArray testVariableWidthBitArray.cc)
gossamer_unit_test(testWordyBitVector testWordyBitVector.cc)
gossamer_unit_test(testVByteCodec testVByteCodec.cc)
gossamer_unit_test(testGossCmdBuildGraph testGossCmdBuildGraph.cc gossapp)
gossamer_unit_test(testGossCmdPrintContigs testGossCmdPrintContigs.cc gossapp)

endif(BUILD_tests)
