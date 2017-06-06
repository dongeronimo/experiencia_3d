//#include "myITKMprActor.h"
//#include <vtkObjectFactory.h>
//#include <vtkViewport.h>
//#include <vtkRenderer.h>
//#include <vtkMatrix4x4.h>
//#include <vtkCamera.h>
//
//#include <iostream>
//#include <array>
//#include <thread>
//
//#include <itkTimeProbe.h>
//
//using namespace std;
//
//vtkStandardNewMacro(MyITKMPRActor);
//
//void MyITKMPRActor::SetVector(array<double, 3> vec)
//{
//	this->vetorNormalAoPlano = vec;
//	isModified = true;
//}
//
//void MyITKMPRActor::SetITKImage(Input3DImageType::Pointer i)
//{
//	InputToInternalCasterType::Pointer caster = InputToInternalCasterType::New();
//	caster->SetInput(i);
//	caster->Update();
//	image = caster->GetOutput();
//	image->DisconnectPipeline();
//}
//
//MyITKMPRActor::MyITKMPRActor()
//{
//
//
//	wlMIP = WLFilterType::New();
//	vetorNormalAoPlano = { 1, 0, 0 };
//	image = nullptr;
//	isModified = true;
//	importFromITKToVTK = nullptr;
//	mipFilter = MipFilterType::New();
//	slabWriter = SlabWriterType::New();
//	writerMIP = WriterType::New();
//	tileFilter = TileFilterType::New();
//	itk::FixedArray<unsigned int, 3> tileLayout;
//	tileLayout[0] = 1; tileLayout[1] = 1; tileLayout[2] = 0;
//	tileFilter->SetLayout(tileLayout);
//}
//
//MyITKMPRActor::~MyITKMPRActor()
//{
//	glDeleteTextures(1, &texture);
//	image = nullptr;
//}
//
//void MyITKMPRActor::PrintSelf(ostream& os, vtkIndent indent)
//{
//	os << "----" << __FUNCTION__ << "----";
//	if (image) image->Print(os, 1); 
//	else os << "Sem imagem ITK" << endl;
//
//	//this->Superclass::PrintSelf(os, indent);
//}
//
//void MyITKMPRActor::ShallowCopy(MyITKMPRActor* t)
//{
//	t->image = this->image;
//}
////Vou implementar essa renderização aqui pra começar
//void MyITKMPRActor::UpdateReslice()
//{
//	try
//	{
//		long t0 = GetCurrentTime();
//		cout << __FUNCTION__ << endl;
//		Internal3DImageType::SizeType size;
//		size = image->GetLargestPossibleRegion().GetSize();
//		Internal3DImageType::SpacingType spacing;
//		spacing = image->GetSpacing();
//		array<double, 3> directionVector = vetorNormalAoPlano;
//		array<double, 3> centerPoint = { -8.0427, -55.007, -217.76 };//O centro do reslice (da marching man)
//		const unsigned int quantosPlanos = 15; //Eu cortarei várias fatias;
//		const double slabSliceDistance = (image->GetSpacing()[0] + image->GetSpacing()[1] + image->GetSpacing()[2])/3; //O espaço entre as fatias;
//		vector<ImageResliceResultType::Pointer> fatias;
//		for (unsigned int i = 0; i < quantosPlanos; i++)
//		{
//			if ( i >= cacheOfObliqueFilters.size())
//			{
//				cacheOfObliqueFilters.push_back(ObliqueFilterType::New());
//			}
//			ObliqueFilterType::InputPointType center;
//			center[0] = centerPoint[0] + i * slabSliceDistance * directionVector[0];
//			center[1] = centerPoint[1] + i * slabSliceDistance * directionVector[1];
//			center[2] = centerPoint[2] + i * slabSliceDistance * directionVector[2];
//			ObliqueFilterType::InputVectorType direction;
//			direction[0] = directionVector[0]; direction[1] = directionVector[1]; direction[2] = directionVector[2];
//			ObliqueFilterType::InputVectorType up;
//			up[0] = 1; up[1] = 0; up[2] = 0;
//			ObliqueFilterType::OutputSizeType outputSize;
//			outputSize[0] = 256; //image->GetLargestPossibleRegion().GetSize()[0];
//			outputSize[1] = 256; //image->GetLargestPossibleRegion().GetSize()[1];
//			ObliqueFilterType::OutputSpacingType outputSpacing;
//			outputSpacing[0] = image->GetSpacing()[0];
//			outputSpacing[1] = image->GetSpacing()[1];
//			cacheOfObliqueFilters[i]->SetNumberOfThreads(thread::hardware_concurrency());
//			cacheOfObliqueFilters[i]->SetInput(image);
//			cacheOfObliqueFilters[i]->SetPlane(center, direction, up);
//			cacheOfObliqueFilters[i]->SetOutputSize(outputSize);
//			cacheOfObliqueFilters[i]->SetOutputSpacing(outputSpacing);
//			tileFilter->SetInput(i, cacheOfObliqueFilters[i]->GetOutput());
//		}
//		mipFilter->SetInput(tileFilter->GetOutput());
//		mipFilter->SetProjectionDimension(2);
//		wlMIP->SetInput(mipFilter->GetOutput());
//		wlMIP->SetWindowMinimum(-150); 
//		wlMIP->SetWindowMaximum(200);
//		wlMIP->SetOutputMinimum(0);
//		wlMIP->SetOutputMaximum(255);
//		wlMIP->Update();
//		this->finalResult = wlMIP->GetOutput();
//		this->finalResult->DisconnectPipeline();
//		if (!importFromITKToVTK)
//		{
//			importFromITKToVTK = vtkSmartPointer<vtkImageImport>::New();
//		}
//		long t1 = GetCurrentTime();
//		cout << "DT = " << (t1 - t0) << endl;
//		slabWriter->SetFileName("C:\\meus dicoms\\slab.mha");
//		slabWriter->SetInput(tileFilter->GetOutput());
//		slabWriter->Write();
//		WriterType::Pointer writerMIP = WriterType::New();
//		writerMIP->SetInput(wlMIP->GetOutput());
//		stringstream ss;
//		ss << "C:\\meus dicoms\\TESTE_" << "MIP" << ".PNG";
//		writerMIP->SetFileName(ss.str().c_str());
//		writerMIP->Write();
//	}
//	catch (itk::ExceptionObject &ex)
//	{
//		ex.Print(cout);
//	}
//}
//
//int MyITKMPRActor::RenderOpaqueGeometry(vtkViewport *v)
//{
//	static bool glInitialized = false;
//	if (!glInitialized)
//	{
//		glDisable(GL_CULL_FACE);
//#ifdef OPENGL_2
//		//TODO: Jogar isso tudo para métodos especializados, lembrando que tem que ser executado com um contexto opengl existente (ou seja, depois do show());
//		string vertexSrc = "#version 330 core\n"
//			"layout(location = 0) in vec2 vertexUV;\n"
//			"layout(location = 1) in vec3 vertexPosition_modelspace;\n"
//			"uniform mat4 mvp;\n"
//			"uniform sampler2D textureSampler;\n"
//			"out vec2 UV;\n"
//			"void main()\n"
//			"{\n"
//			"  gl_Position = mvp * vec4(vertexPosition_modelspace, 1);\n"
//			"  UV = vertexUV;\n"
//			"}\n";
//		string fragSrc = "#version 330 core\n"
//			"in vec2 UV;\n"
//			"out vec4 color;\n"
//			"uniform sampler2D textureSampler;\n"
//			"void main()\n"
//			"{\n"
//			"  vec3 tx = texture(textureSampler, UV).rgb; \n"
//			"  color = vec4(tx.x, tx.x, tx.x, 1);\n"
//			"}\n";
//		myShader = make_unique<MyShader>(vertexSrc, fragSrc);
//#endif
//		const float min_vert = -0.2; //0
//		const float max_vert = 0.2;
//
//		std::array<float, 3> p1 = { min_vert, min_vert, 0 };  std::array<float, 2> uv1 = { 0, 0 };
//		std::array<float, 3> p2 = { max_vert, min_vert, 0 };  std::array<float, 2> uv2 = { 1, 0 };
//		std::array<float, 3> p3 = { min_vert, max_vert, 0 };  std::array<float, 2> uv3 = { 0, 1 };
//		std::array<float, 3> p4 = { max_vert, max_vert, 0 };  std::array<float, 2> uv4 = { 1, 1 };
//
//		std::vector<std::array<float, 3>> vb; std::vector<std::array<float, 2>> uv;
//		std::array<float, 3> v1 = { p1[0], p1[1], p1[2] }; 
//		std::array<float, 3> v2 = { p2[0], p2[1], p2[2] }; 
//		std::array<float, 3> v3 = { p3[0], p3[1], p3[2] }; 
//		std::array<float, 3> v4 = { p4[0], p4[1], p4[2] }; 
//		vb.push_back(v1); uv.push_back(uv1);
//		vb.push_back(v2); uv.push_back(uv2);
//		vb.push_back(v3); uv.push_back(uv3);
//
//		vb.push_back(v2); uv.push_back(uv2);
//		vb.push_back(v4); uv.push_back(uv4);
//		vb.push_back(v3); uv.push_back(uv3);
//		
//		myGeometry = make_unique<MyGeometry>(vb, uv);
//		glInitialized = true;
//	}
//	if (isModified)
//	{
//		UpdateReslice();
//		static int textureSize = -1;		
//		long t0 = GetCurrentTime();
//		if (textureSize != finalResult->GetLargestPossibleRegion().GetSize()[0])
//		{
//			cout << "criando a textura pq o tamanho mudou" << endl;
//			textureSize = finalResult->GetLargestPossibleRegion().GetSize()[0];
//			glDeleteTextures(1, &texture);
//			glGenTextures(1, &texture);
//			glBindTexture(GL_TEXTURE_2D, texture);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//#ifdef OPENGL_2
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//#endif
//		}
//		const void *buffer = finalResult->GetBufferPointer();
//
//		glTexImage2D(GL_TEXTURE_2D,
//			0,
//			GL_RED,
//			textureSize,
//			textureSize,
//			0,
//			GL_RED,
//			GL_UNSIGNED_BYTE,
//			buffer
//		);
//		long t1 = GetCurrentTime();
//		cout << "TRANSFERENCIA DA TEXTURA = " << (t1 - t0) << " MS" << endl;
//		isModified = false;
//	}
//	long t0 = GetCurrentTime();
//	vtkRenderer *ren = vtkRenderer::SafeDownCast(v);
//	vtkCamera *cam = ren->GetActiveCamera();
//	vtkSmartPointer<vtkMatrix4x4> mvp = cam->GetModelViewTransformMatrix();
//#ifdef OPENGL_2
//	myGeometry->Render(mvp, myShader.get(), texture);
//#else
//	myGeometry->Render(mvp, texture);
//#endif
//	long t1 = GetCurrentTime();
//	cout << "RENDERIZACAO = " << (t1 - t0) << endl;
//	return 1;
//}
////No futuro tb posso usar essa renderização aqui pra fazer coisas uteis
//int MyITKMPRActor::RenderOverlay(vtkViewport *v)
//{
//	cout << __FUNCTION__ << endl;
//	return 0;
//}
//
//int MyITKMPRActor::RenderTranslucentPolygonalGeometry(vtkViewport *v)
//{
////	cout << __FUNCTION__ << endl;
//	return 0;
//}
//int MyITKMPRActor::RenderVolumetricGeometry(vtkViewport *v)
//{
////	cout << __FUNCTION__ << endl;
//	return 0;
//}
//bool MyITKMPRActor::RenderFilteredOpaqueGeometry(vtkViewport *v, vtkInformation *requiredKeys)
//{
////	cout << __FUNCTION__ << endl;
//	return false;
//}
//bool MyITKMPRActor::RenderFilteredTranslucentPolygonalGeometry(vtkViewport *v, vtkInformation *requiredKeys)
//{
////	cout << __FUNCTION__ << endl;
//	return false;
//}
//bool MyITKMPRActor::RenderFilteredVolumetricGeometry(vtkViewport *v, vtkInformation *requiredKeys)
//{
////	cout << __FUNCTION__ << endl;
//	return false;
//}
//bool MyITKMPRActor::RenderFilteredOverlay(vtkViewport *v, vtkInformation *requiredKeys)
//{
////	cout << __FUNCTION__ << endl;
//	return false;
//}
//
