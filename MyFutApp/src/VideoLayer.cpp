#include "VideoLayer.h"
#include "imgui.h"

VideoLayer::VideoLayer(const std::string& videoPath)
	: Layer("VideoLayer"), m_videoPath(videoPath)
{
	CY_ASSERT(!videoPath.empty(), "Empty video path!!");
	m_squarePosition = { 0.0f, 0.0f, 0.0f };
	m_videoCapture = cv::VideoCapture(m_videoPath);
	if (!m_videoCapture.isOpened())
	{
		CY_ERROR("Could not open video stream");
		m_frameRate = 0;
	}
	else
	{
		//Consult cv::VideoCaptureProperties for the available properties
		m_frameRate = m_videoCapture.get(cv::CAP_PROP_FPS);
	}
	auto assetsPath = std::filesystem::current_path() / "assets";
#ifdef USE_CUDA
	m_ObjectDetector = std::make_shared<ObjectDetector>(assetsPath.string(), true);
#else
	m_ObjectDetector = std::make_shared<ObjectDetector>(assetsPath.string(), false);
#endif
}

VideoLayer::~VideoLayer()
{
	if (m_videoCapture.isOpened())
	{
		m_videoCapture.release();
	}
	m_ObjectDetector = nullptr;
}

void VideoLayer::OnAttach()
{
	cv::Mat img;
	m_videoCapture.read(img);
	while (img.empty())
	{
		//CY_ERROR("Video frame is empty");
		m_videoCapture.read(img);
	}
	cv::flip(img, img, 0);
	cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
	m_currentFrame = CatolYeah::Texture2D::Create(img.cols, img.rows, img.channels(), 0);
	m_currentFrame->SetData(img.data, img.cols * img.rows * img.channels());
}

void VideoLayer::OnUpdate(CatolYeah::Timestep ts)
{
	// Square translation
	if (CatolYeah::Input::IsKeyPressed(CY_KEY_L))
		m_squarePosition.x += 5.0f * ts;
	else if (CatolYeah::Input::IsKeyPressed(CY_KEY_J))
		m_squarePosition.x -= 5.0f * ts;

	if (CatolYeah::Input::IsKeyPressed(CY_KEY_I))
		m_squarePosition.y += 5.0f * ts;
	else if (CatolYeah::Input::IsKeyPressed(CY_KEY_K))
		m_squarePosition.y -= 5.0f * ts;

	static float frameTime = 0.0f;
	frameTime += ts.GetMilliseconds();
	if (frameTime > 1000.0f/m_frameRate)
	{
		cv::Mat img;
		if (m_videoCapture.read(img) && !img.empty())
		{
			//m_videoCapture.read(img);
			cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
			cv::flip(img, img, 0);
			m_ObjectDetector->Detect(img);
			m_currentFrame->SetData(img.data, img.cols * img.rows * img.channels());
		}
		frameTime = 0.0f;
	}

	// Render commands
	CatolYeah::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	CatolYeah::RenderCommand::Clear();

	CatolYeah::Renderer2D::BeginScene();

	//CY_INFO("Square position [x:{0}] [y:{1}", m_squarePosition.x, m_squarePosition.y);
	CatolYeah::Renderer2D::DrawQuad({ m_squarePosition.x, m_squarePosition.y, 0.2f }, { 1.0f, 1.0f }, m_currentFrame);				// Texture closes to the screen than flat color quad
	CatolYeah::Renderer2D::EndScene();
}

void VideoLayer::OnEvent(CatolYeah::Event& e)
{
	CatolYeah::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<CatolYeah::MouseButtonPressedEvent>(CY_BIND_EVENT_FN(VideoLayer::m_MouseEventHandler));
}

bool VideoLayer::m_MouseEventHandler(CatolYeah::MouseButtonPressedEvent& e)
{
	CY_DEBUG("Mouse click pos: {0}x{1}", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
	return EVENT_RETURN_PASS_ON;
}
