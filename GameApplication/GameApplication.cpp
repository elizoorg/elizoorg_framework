#include "GameApplication.h"

	GameApplication::GameApplication()
	{
		instance = this;
		Device = new InputDevice(this);
	}
	GameApplication* GameApplication::instance = nullptr;
	GameApplication::~GameApplication()
	{
	}


	int GameApplication::Run() {

		_display = new WinApi_Display(this);
		_display->CreateDisplay();
		//_display.OnMouseMove += [](InputDevice::RawMouseEventArgs args) {InputDevice::Instance().OnMouseMove(args); };
		//_display.OnKeyDown += [](InputDevice::KeyboardInputEventArgs args) {InputDevice::Instance().OnKeyDown(args); };





		Initialize();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(_display->getHWND());
		ImGui_ImplDX11_Init(device.Get(), context);
		ImGui::StyleColorsDark();
		io.WantCaptureMouse = true;


		while (!isClosed) {
			UpdateInternal();
			Update();
			Draw();
		};


		return 0;

	}

	void GameApplication::CreateBackBuffer()
	{
	}

	void GameApplication::DestroyResources()
	{
	}

	void GameApplication::Draw()
	{

		context->OMSetRenderTargets(1, &rtv, depthStencilView.Get());



		float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		context->ClearRenderTargetView(rtv, color);
		context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		D3D11_VIEWPORT viewport = {};
		viewport.Width = static_cast<float>(_display->getWidth());
		viewport.Height = static_cast<float>(_display->getHeight());
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;

		context->RSSetViewports(1, &viewport);




		for (auto& comp : Components)
			comp->Draw();


		
		bool qq = false;


		




		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		ImGui::Begin("Testestestestestestest");
		bool isHovered = ImGui::IsItemHovered();
		bool isFocused = ImGui::IsItemFocused();
		ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
		ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
		ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);
		ImGui::Text("Is mouse over screen? %s", isHovered ? "Yes" : "No");
		ImGui::Text("Is screen focused? %s", isFocused ? "Yes" : "No");
		ImGui::Text("Position: %f, %f", mousePositionRelative.x, mousePositionRelative.y);
		ImGui::Text("Mouse clicked: %s", ImGui::IsMouseDown(ImGuiMouseButton_Left) ? "Yes" : "No");


		ImGui::End();
		ImGui::Render();


		context->OMSetRenderTargets(1, &rtv, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		context->OMSetRenderTargets(0, nullptr, nullptr);






		swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);




	}

	void GameApplication::EndFrame()
	{
	}

	void GameApplication::Exit()
	{
	}

	void GameApplication::Initialize()
	{
		Device = new InputDevice(this);
		camera = new Camera(this);
		camera->SetPosition(0, 0, 200);
		camera->Bind();
		swapDesc.BufferCount = 2;
		swapDesc.BufferDesc.Width = _display->getWidth();
		swapDesc.BufferDesc.Height = _display->getHeight();
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = _display->getHWND();
		swapDesc.Windowed = true;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;


		D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };
		auto res = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			featureLevel,
			1,
			D3D11_SDK_VERSION,
			&swapDesc,
			&swapChain,
			&device,
			nullptr,
			&context);
		if (FAILED(res))
		{
			std::cout << "So,unexpected shit happens\n";
			// Well, that was unexpected
		}
		res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);	// __uuidof(ID3D11Texture2D)
		if (FAILED(res)) {
			std::cout << "So,unexpected shit happens3\n";
		}
		res = device->CreateRenderTargetView(backTex, nullptr, &rtv);
		if (FAILED(res)) {
			std::cout << "So,unexpected shit happens2\n";
		}

		TriangleComponent* trag = new TriangleComponent(this);
		TriangleComponent* trag1 = new TriangleComponent(this);
		TriangleComponent* trag2 = new TriangleComponent(this);
		SphereComponent* sphere = new SphereComponent(this);
		SphereComponent* sphere2 = new SphereComponent(this);
		SphereComponent* sphere3 = new SphereComponent(this);
		Components.push_back(trag);
		Components.push_back(trag1);
		Components.push_back(trag2);
		Components.push_back(sphere);
		Components.push_back(sphere2);
		Components.push_back(sphere3);

		for (auto comp : Components) {
			comp->Initialize();
		}



		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FillMode = D3D11_FILL_WIREFRAME;


		std::srand(std::time(nullptr));

		ResetGame();


		res = device->CreateRasterizerState(&rastDesc, &rastState);
		if (FAILED(res)) {
			std::cout << "So,unexpected shit happens4\n";
		}

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = _display->getWidth();
		depthStencilDesc.Height = _display->getHeight();
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		res = device->CreateTexture2D(&depthStencilDesc, NULL, depthStencilBuffer.GetAddressOf());
		if (FAILED(res)) {
			std::cout << "So,unexpected shit happens5\n";
		}

		res = device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf());
		if (FAILED(res)) {
			std::cout << "So,unexpected shit happens6\n";
		}



		



	}

	void GameApplication::MessageHandler()
	{
	}

	void GameApplication::PrepareFrame()
	{
	}

	void GameApplication::PrepareRecources()
	{
	}

	void GameApplication::RestoreTargets()
	{
	}

	bool GameApplication::Update()
	{

		while (!_display->PollMessages())
		{

		}

		if (Device->IsKeyDown(Keys::Escape))
		{
			isClosed = true;
			return true;
		}
		if (Device->IsKeyDown(Keys::D))
		{
			camera->Strafe(1);
		}
		if (Device->IsKeyDown(Keys::A))
		{
			camera->Strafe(-1);
		}
		if (Device->IsKeyDown(Keys::S))
		{
			camera->Walk(1);
		}
		if (Device->IsKeyDown(Keys::W))
		{
			camera->Walk(-1);
		}
		if (Device->IsKeyDown(Keys::Space))
		{
			camera->Fly(1);
		}
		if (Device->IsKeyDown(Keys::LeftShift))
		{
			camera->Fly(-1);
		}
		if (Device->IsKeyDown(Keys::LeftButton)) {
			ImGuiIO& io = ImGui::GetIO();
			io.AddMouseButtonEvent(0, true);
		}
		else {
			ImGuiIO& io = ImGui::GetIO();
			io.AddMouseButtonEvent(0, false);
		}


		


		if (Device->IsKeyDown(Keys::LeftControl) || Device->IsKeyDown(Keys::RightAlt)) {
			std::cout << "It's working!\n";
		}

		transform[3].setRotationSpeed(1);
		transform[4].setRotationSpeed(-1);
		transform[5].setRotationSpeed(5);



		transform[4].setPosition(transform[3].getPosition() + Vector3::Transform(Vector3(50, 0, 0), Matrix::CreateFromAxisAngle(transform[3].getRotationVector(),
			Math::Radians(transform[3].getRotationAngle().x))));

		transform[5].setPosition(transform[4].getPosition() + Vector3::Transform(Vector3(50, 0, 0), Matrix::CreateFromAxisAngle(transform[3].getRotationVector(),
			Math::Radians(transform[4].getRotationAngle().x))));

		camera->UpdateViewMatrix();


		for (size_t t = 0; t < 6; t++) {
			transform[t].Update();
		}


		for (size_t t = 0; t < Components.size(); t++) {
			Components[t]->Update(camera->ViewProj(), transform[t].getPosition(), transform[t].getScale(),
				Matrix::CreateTranslation(Vector3(50, 50, 0)) * Matrix::CreateFromAxisAngle(transform[t].getRotationVector(), Math::Radians(transform[t].getRotationAngle().x)));
		}

		return true;

	}

	void GameApplication::UpdateInternal()
	{
		auto	curTime = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
		PrevTime = curTime;

		totalTime += deltaTime;
		frameCount++;

		if (totalTime > 1.0f) {
			float fps = frameCount / totalTime;

			totalTime -= 1.0f;

			WCHAR text[256];
			swprintf_s(text, TEXT("FPS: %f"), fps);
			SetWindowText(_display->getHWND(), text);

			frameCount = 0;
		}
	}
	void GameApplication::ResetGame()
	{
		transform[0].setPosition(Vector3(100, 0, 200));
		transform[1].setPosition(Vector3(0, 0, 0));

		transform[0].setScale(Vector3(1, 10, 1));
		transform[1].setScale(Vector3(1, 10, 1));
		transform[2].setScale(Vector3(1.0f, 1.0f, 1.0f));
		transform[2].setPosition(Vector3(0, 0, 0));
		transform[3].setRotationAngle(Vector3(1, 0, 0));
		transform[3].setPosition(Vector3(0, 0, 0));
		transform[3].setScale(Vector3(10.0f, 10.0f, 10.0f));

		transform[3].setRotationVector(Vector3(1, 0, 0));


		transform[4].setRotationVector(Vector3(((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX))));
		transform[5].setRotationVector(Vector3(((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX))));

		transform[0].setRotationVector(Vector3(1, 0, 0));
		transform[1].setRotationVector(Vector3(0, 1, 0));
		transform[2].setRotationVector(Vector3(0, 0, 1));

		transform[0].setRotationSpeed(Vector3(8, 0, 0));
		transform[1].setRotationSpeed(Vector3(2, -5, 0));
		transform[2].setRotationSpeed(Vector3(-10, 0, 1));

		transform[0].setRotationAngle(Vector3(0, 1, 1));
		transform[1].setRotationAngle(Vector3(1, 0, 1));
		transform[2].setRotationAngle(Vector3(0, 0, 1));

		transform[4].setPosition(Vector3(0, 0, 0));
		transform[4].setScale(Vector3(10.0f, 10.0f, 10.0f));

		transform[5].setPosition(Vector3(0, 0, 0));
		transform[5].setScale(Vector3(10.0f, 10.0f, 10.0f));
	}

