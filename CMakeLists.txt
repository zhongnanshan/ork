PROJECT( Ork )

CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

# 设置外部依赖库

FIND_PATH( PThreadInclude pthread.h)
FIND_LIBRARY( PThreadLib pthreadVCE2.lib)

MESSAGE( ${PThreadInclude} )
MESSAGE( ${PThreadLib} )

FIND_PATH( GLUTInclude GL/glut.h)
FIND_LIBRARY( GLUTLib glut32.lib)

MESSAGE( ${GLUTInclude} )
MESSAGE( ${GLUTLib} )

FIND_PATH( GLEWInclude GL/glew.h)
FIND_LIBRARY( GLEWLib glew32.lib)

MESSAGE( ${GLEWInclude} )
MESSAGE( ${GLEWLib} )

# 定义源代码集

ADD_DEFINITIONS( 
	-DTIXML_USE_STL 
	-DSTBI_NO_STDIO 
	-DSTBI_NO_WRITE 
	-DGLUT_NO_WARNING_DISABLE 
	-DGLUT_NO_LIB_PRAGMA 
	-D_STDCALL_SUPPORTED 
	-D_CRT_SECURE_NO_WARNINGS	
	-D_M_IX86 
	-DHAVE_STRUCT_TIMESPEC
#	-DUSE_SHARED_PTR
	)

INCLUDE_DIRECTORIES( 
	${PROJECT_SOURCE_DIR} 
	${PThreadInclude} 
	${GLUTInclude} 
	${GLEWInclude}
	libraries
	libraries/tibyxml
	libraries/stbi
	)

SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

SET( ORK_SRCS
		libraries/pmath.h
		libraries/stbi/stb_image.cpp
		libraries/stbi/stb_image.h
		libraries/tinyxml/tinyxml.cpp
		libraries/tinyxml/tinyxml.h
		libraries/tinyxml/tinyxmlerror.cpp
		libraries/tinyxml/tinyxmlparser.cpp
		ork/core/Atomic.h
		ork/core/Factory.h
		ork/core/FileLogger.cpp
		ork/core/FileLogger.h
		ork/core/GPUTimer.cpp
		ork/core/GPUTimer.h
		ork/core/Iterator.h
		ork/core/Logger.cpp
		ork/core/Logger.h
		ork/core/Object.cpp
		ork/core/Object.h
		ork/core/Timer.cpp
		ork/core/Timer.h
		ork/math/box2.h
		ork/math/box3.h
		ork/math/half.cpp
		ork/math/half.h
		ork/math/mat2.h
		ork/math/mat3.h
		ork/math/mat4.h
		ork/math/quat.h
		ork/math/vec2.h
		ork/math/vec3.h
		ork/math/vec4.h
		ork/render/AttributeBuffer.cpp
		ork/render/AttributeBuffer.h
		ork/render/Buffer.cpp
		ork/render/Buffer.h
		ork/render/CPUBuffer.cpp
		ork/render/CPUBuffer.h
		ork/render/FrameBuffer.cpp
		ork/render/FrameBuffer.h
		ork/render/GPUBuffer.cpp
		ork/render/GPUBuffer.h
		ork/render/Mesh.h
		ork/render/MeshBuffers.cpp
		ork/render/MeshBuffers.h
		ork/render/Module.cpp
		ork/render/Module.h
		ork/render/Program.cpp
		ork/render/Program.h
		ork/render/Query.cpp
		ork/render/Query.h
		ork/render/RenderBuffer.cpp
		ork/render/RenderBuffer.h
		ork/render/Sampler.cpp
		ork/render/Sampler.h
		ork/render/Texture.cpp
		ork/render/Texture.h
		ork/render/Texture1D.cpp
		ork/render/Texture1D.h
		ork/render/Texture1DArray.cpp
		ork/render/Texture1DArray.h
		ork/render/Texture2D.cpp
		ork/render/Texture2D.h
		ork/render/Texture2DArray.cpp
		ork/render/Texture2DArray.h
		ork/render/Texture2DMultisample.cpp
		ork/render/Texture2DMultisample.h
		ork/render/Texture2DMultisampleArray.cpp
		ork/render/Texture2DMultisampleArray.h
		ork/render/Texture3D.cpp
		ork/render/Texture3D.h
		ork/render/TextureBuffer.cpp
		ork/render/TextureBuffer.h
		ork/render/TextureCube.cpp
		ork/render/TextureCube.h
		ork/render/TextureCubeArray.cpp
		ork/render/TextureCubeArray.h
		ork/render/TextureRectangle.cpp
		ork/render/TextureRectangle.h
		ork/render/TransformFeedback.cpp
		ork/render/TransformFeedback.h
		ork/render/Types.cpp
		ork/render/Types.h
		ork/render/Uniform.cpp
		ork/render/Uniform.h
		ork/render/Value.cpp
		ork/render/Value.h
		ork/resource/CompiledResourceLoader.cpp
		ork/resource/CompiledResourceLoader.h
		ork/resource/Resource.cpp
		ork/resource/Resource.h
		ork/resource/ResourceCompiler.cpp
		ork/resource/ResourceCompiler.h
		ork/resource/ResourceDescriptor.cpp
		ork/resource/ResourceDescriptor.h
		ork/resource/ResourceFactory.cpp
		ork/resource/ResourceFactory.h
		ork/resource/ResourceLoader.cpp
		ork/resource/ResourceLoader.h
		ork/resource/ResourceManager.cpp
		ork/resource/ResourceManager.h
		ork/resource/ResourceTemplate.h
		ork/resource/XMLResourceLoader.cpp
		ork/resource/XMLResourceLoader.h
		ork/scenegraph/AbstractTask.cpp
		ork/scenegraph/AbstractTask.h
		ork/scenegraph/CallMethodTask.cpp
		ork/scenegraph/CallMethodTask.h
		ork/scenegraph/DrawMeshTask.cpp
		ork/scenegraph/DrawMeshTask.h
		ork/scenegraph/LoopTask.cpp
		ork/scenegraph/LoopTask.h
		ork/scenegraph/Method.cpp
		ork/scenegraph/Method.h
		ork/scenegraph/SceneManager.cpp
		ork/scenegraph/SceneManager.h
		ork/scenegraph/SceneNode.cpp
		ork/scenegraph/SceneNode.h
		ork/scenegraph/SequenceTask.cpp
		ork/scenegraph/SequenceTask.h
		ork/scenegraph/SetProgramTask.cpp
		ork/scenegraph/SetProgramTask.h
		ork/scenegraph/SetStateTask.cpp
		ork/scenegraph/SetStateTask.h
		ork/scenegraph/SetTargetTask.cpp
		ork/scenegraph/SetTargetTask.h
		ork/scenegraph/SetTransformsTask.cpp
		ork/scenegraph/SetTransformsTask.h
		ork/scenegraph/ShowInfoTask.cpp
		ork/scenegraph/ShowInfoTask.h
		ork/scenegraph/ShowLogTask.cpp
		ork/scenegraph/ShowLogTask.h
		ork/taskgraph/MultithreadScheduler.cpp
		ork/taskgraph/MultithreadScheduler.h
		ork/taskgraph/Scheduler.cpp
		ork/taskgraph/Scheduler.h
		ork/taskgraph/Task.cpp
		ork/taskgraph/Task.h
		ork/taskgraph/TaskFactory.cpp
		ork/taskgraph/TaskFactory.h
		ork/taskgraph/TaskGraph.cpp
		ork/taskgraph/TaskGraph.h
		ork/ui/EventHandler.cpp
		ork/ui/EventHandler.h
		ork/ui/GlutWindow.cpp
		ork/ui/GlutWindow.h
		ork/ui/Window.cpp
		ork/ui/Window.h
		ork/util/Font.cpp
		ork/util/Font.h
	)

SET( EXAM_SRCS
		examples/Main.cpp
		examples/Main.h
		examples/MinimalExample.cpp
		examples/RenderExample.cpp
		examples/ResourceExample.cpp
		examples/SceneGraphExample.cpp
		examples/SceneGraphResourceExample.cpp
		examples/TessellationExample.cpp
	)

SET( TEST_SRCS
		test/Test.cpp
		test/Test.h
		test/TestFrameBuffer.cpp
		test/TestResource.cpp
		test/TestTexture.cpp
		test/TestUniform.cpp
		test/TestUniformBlock.cpp
	)

SET( EXAM_ALL_SRCS ${EXAM_SRCS} ${ORK_SRCS} )

SET( TEST_ALL_SRCS ${TEST_SRCS} ${ORK_SRCS} )

# 定义生成项目

ADD_LIBRARY( Ork31 STATIC ${ORK_SRCS} )
TARGET_LINK_LIBRARIES( Ork31 
	${PThreadLib} 
	${GLUTLib} 
	${GLEWLib}
	opengl32.lib
	glu32.lib
	)
IF( MSVC )
#	TARGET_COMPILE_OPTIONS( Ork31 PRIVATE "-DORK_API=__declspec(dllexport)" )
	TARGET_COMPILE_OPTIONS( Ork31 PRIVATE "-DORK_API=" )
	TARGET_COMPILE_OPTIONS( Ork31 PRIVATE /wd"4002" /wd"4996" /wd"4244" /wd"4251" /wd"4800" )
ENDIF( MSVC )

ADD_EXECUTABLE( Exams ${EXAM_SRCS} )
TARGET_LINK_LIBRARIES( Exams Ork31 )
IF( MSVC )
#	TARGET_COMPILE_OPTIONS( Exams PRIVATE "-DORK_API=__declspec(dllimport)" )
	TARGET_COMPILE_OPTIONS( Exams PRIVATE "-DORK_API=" )
	TARGET_COMPILE_OPTIONS( Exams PRIVATE /wd"4002" /wd"4244" /wd"4251" /wd"4800" )
ENDIF( MSVC )
	
ADD_EXECUTABLE( Tests ${TEST_SRCS} )
TARGET_LINK_LIBRARIES( Tests Ork31 )
IF( MSVC )
#	TARGET_COMPILE_OPTIONS( Tests PRIVATE "-DORK_API=__declspec(dllimport)" )
	TARGET_COMPILE_OPTIONS( Tests PRIVATE "-DORK_API=" )
	TARGET_COMPILE_OPTIONS( Tests PRIVATE /wd"4002" /wd"4244" /wd"4251" /wd"4800" )
ENDIF( MSVC )

ADD_SUBDIRECTORY( myexams )

#ADD_EXECUTABLE( ExamsAll ${EXAM_ALL_SRCS} )
#TARGET_LINK_LIBRARIES( ExamsAll 
#	${PThreadLib} 
#	${GLUTLib} 
#	${GLEWLib}
#	opengl32.lib
#	glu32.lib
#	)
#IF( MSVC )
#	TARGET_COMPILE_OPTIONS( ExamsAll PRIVATE "-DORK_API=" )
#	TARGET_COMPILE_OPTIONS( ExamsAll PRIVATE /wd"4002" /wd"4996" /wd"4244" /wd"4251" /wd"4800" )
#ENDIF( MSVC )

#ADD_EXECUTABLE( TestsAll ${TEST_ALL_SRCS} )
#TARGET_LINK_LIBRARIES( TestsAll 
#	${PThreadLib} 
#	${GLUTLib} 
#	${GLEWLib}
#	opengl32.lib
#	glu32.lib
#	)
#IF( MSVC )
#	TARGET_COMPILE_OPTIONS( TestsAll PRIVATE "-DORK_API=" )
#	TARGET_COMPILE_OPTIONS( TestsAll PRIVATE /wd"4002" /wd"4996" /wd"4244" /wd"4251" /wd"4800" )
#ENDIF( MSVC )
