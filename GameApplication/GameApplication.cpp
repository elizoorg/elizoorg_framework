#include "GameApplication.h"


	GameApplication::GameApplication()
	{
		instance = this;
		Device = new InputDevice(this);
		
	}
	GameApplication* GameApplication::instance = nullptr;
	GameApplication::~GameApplication()
	{

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		for (auto comp : Components) {
			delete comp;
		}
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
		float color[] = { 0.6f, 0.6f, 0.6f, 1.0f };
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

		Components[22]->Draw();
		
		for (size_t t = 0; t < Components.size() - 1; t++)
		{
			Components[t]->Draw();
		}



		
		system->Draw(deltaTime);
		
		bool qq = false;


		




		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

	

		ImGui::Begin("Testesteste4343242stestestest");
		bool isHovered = ImGui::IsItemHovered();
		bool isFocused = ImGui::IsItemFocused();
		ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
		ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
		ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);
		ImGui::Text("Is mouse over screen? %s", isHovered ? "Yes" : "No");
		ImGui::Text("Is screen focused? %s", isFocused ? "Yes" : "No");
		ImGui::Text("Position: %f, %f", mousePositionRelative.x, mousePositionRelative.y);
		ImGui::Text("Mouse clicked: %s", ImGui::IsMouseDown(ImGuiMouseButton_Left) ? "Yes" : "No");


		ImGui::Text("%f %f %f %f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z,1.0f);

		ImGui::Text("%f %f %f %f", transform[0].GetWorldPosition().x, transform[0].GetWorldPosition().y, transform[0].GetWorldPosition().z, planets[0].radius);
		ImGui::Text("%f %f %f %f", transform[1].GetWorldPosition().x, transform[1].GetWorldPosition().y, transform[1].GetWorldPosition().z, planets[1].radius);
		ImGui::Text("%f %f %f %f", transform[2].GetWorldPosition().x, transform[2].GetWorldPosition().y, transform[2].GetWorldPosition().z, planets[2].radius);
		ImGui::Text("%f %f %f %f", transform[3].GetWorldPosition().x, transform[3].GetWorldPosition().y, transform[3].GetWorldPosition().z, planets[3].radius);
		ImGui::Text("%f %f %f %f", transform[4].GetWorldPosition().x, transform[4].GetWorldPosition().y, transform[4].GetWorldPosition().z, planets[4].radius);
		ImGui::Text("%f %f %f %f", transform[5].GetWorldPosition().x, transform[5].GetWorldPosition().y, transform[5].GetWorldPosition().z, planets[5].radius);
		ImGui::Text("%f %f %f %f", transform[6].GetWorldPosition().x, transform[6].GetWorldPosition().y, transform[6].GetWorldPosition().z, planets[6].radius);



		ImGui::SliderFloat("Test", &angle, 0.0f, 6.28f);
		ImGui::SliderFloat("Platen0", &planets[0].angle, 0.0f, 360.28f);
		ImGui::SliderFloat("Platen1", &planets[1].angle, 0.0f, 360.28f);
		ImGui::SliderFloat("Platen2", &planets[2].angle, 0.0f, 360.28f);
		ImGui::SliderFloat("Platen3", &planets[3].angle, 0.0f, 360.28f);
		ImGui::SliderFloat("Platen4", &planets[4].angle, 0.0f, 360.28f);
		ImGui::SliderFloat("Platen5", &planets[5].angle, 0.0f, 360.28f);
		ImGui::SliderFloat("Platen6", &planets[6].angle, 0.0f, 360.28f);


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
		camera->SetPosition(0, 0, 0);
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

		SphereComponent* sphere = new SphereComponent(this);
		SphereComponent* sphere2 = new SphereComponent(this);
		SphereComponent* sphere3 = new SphereComponent(this);
		SphereComponent* sphere4 = new SphereComponent(this);
		SphereComponent* sphere5 = new SphereComponent(this);
		SphereComponent* sphere6 = new SphereComponent(this);
		SphereComponent* sphere7 = new SphereComponent(this);


		TriangleComponent* cube = new TriangleComponent(this);
		TriangleComponent* cube2 = new TriangleComponent(this);
		TriangleComponent* cube3 = new TriangleComponent(this);
		TriangleComponent* cube4 = new TriangleComponent(this);
		TriangleComponent* cube5 = new TriangleComponent(this);
		TriangleComponent* cube6 = new TriangleComponent(this);
		TriangleComponent* cube7 = new TriangleComponent(this);
		TriangleComponent* cube8 = new TriangleComponent(this);
		TriangleComponent* cube9 = new TriangleComponent(this);
		TriangleComponent* cube10 = new TriangleComponent(this);
		TriangleComponent* cube11 = new TriangleComponent(this);
		TriangleComponent* cube12 = new TriangleComponent(this);
		TriangleComponent* cube13 = new TriangleComponent(this);
		TriangleComponent* cube14 = new TriangleComponent(this);
		TriangleComponent* cube15 = new TriangleComponent(this);
		PlaneComponent* plane = new PlaneComponent(this);

		Components.push_back(sphere);
		Components.push_back(sphere2);
		Components.push_back(sphere3);
		Components.push_back(sphere4);
		Components.push_back(sphere5);
		Components.push_back(sphere6);
		Components.push_back(sphere7);

		Components.push_back( cube );
		Components.push_back( cube2 );
		Components.push_back( cube3 );
		Components.push_back( cube4 );
		Components.push_back( cube5 );
		Components.push_back( cube6 );
		Components.push_back( cube7 );
		Components.push_back( cube8 );
		Components.push_back( cube9 );
		Components.push_back( cube10);
		Components.push_back( cube11);
		Components.push_back( cube12);
		Components.push_back( cube13);
		Components.push_back( cube14);
		Components.push_back( cube15);

		Components.push_back(plane);


		for (auto comp : Components) {
			comp->Initialize();
		}

		system = new DebugRenderSysImpl(this);
		system->SetCamera(camera);

		rastDesc.CullMode = D3D11_CULL_FRONT;
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
		depthStencilDesc.SampleDesc = { 1,0 };

		D3D11_DEPTH_STENCIL_DESC depthstencildesc;


		depthstencildesc.DepthEnable = true;
		depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		depthstencildesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; //KEEP
		depthstencildesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR; //INCR
		depthstencildesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; //KEEP
		depthstencildesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		depthstencildesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; //KEEP
		depthstencildesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR; //DECR
		depthstencildesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; //KEEP
		depthstencildesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	

		res = device->CreateTexture2D(&depthStencilDesc, NULL, depthStencilBuffer.GetAddressOf());
		if (FAILED(res)) {
			std::cout << "So,unexpected shit happens5\n";
		}

		res = device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf());
		if (FAILED(res)) {
			std::cout << "So,unexpected shit happens6\n";
		}


		res = device->CreateDepthStencilState(&depthstencildesc, depthStencilState.GetAddressOf());
		if (FAILED(res)) {
			std::cout << "So,unexpected shit happens7\n";
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
		context->OMSetRenderTargets(1, &rtv, depthStencilView.Get());
	}

	bool GameApplication::Update()
	{

		while (!_display->PollMessages())
		{

		}
		auto tr = camera->getTransform();
		if (Device->IsKeyDown(Keys::Escape))
		{
			isClosed = true;
			return true;
		}
		if (Device->IsKeyDown(Keys::D))
		{
			tr->AdjustPosition(tr->GetRightVector());
		}
		if (Device->IsKeyDown(Keys::A))
		{
			tr->AdjustPosition(tr->GetRightVector()*-1);
		}
		if (Device->IsKeyDown(Keys::S))
		{
			tr->AdjustPosition(tr->GetForwardVector()*-1);
		}
		if (Device->IsKeyDown(Keys::W))
		{
			tr->AdjustPosition(tr->GetForwardVector());
		}
		if (Device->IsKeyDown(Keys::Space))
		{
			tr->AdjustPosition(tr->GetUpVector());
		}
		if (Device->IsKeyDown(Keys::J))
		{
			isMouseUsed = !isMouseUsed;
		}
		if (Device->IsKeyDown(Keys::LeftShift))
		{
			tr->AdjustPosition(tr->GetUpVector()*-1);
		}
		if (Device->IsKeyDown(Keys::LeftButton)) {
			ImGuiIO& io = ImGui::GetIO();
			io.AddMouseButtonEvent(0, true);
		}
		else {
			ImGuiIO& io = ImGui::GetIO();
			io.AddMouseButtonEvent(0, false);
		}
		if (isMouseUsed) {
			SetCursorPos(_display->getWidth() / 2, _display->getHeight() / 2);
		}
	
		


		if (Device->IsKeyDown(Keys::LeftControl) || Device->IsKeyDown(Keys::RightAlt)) {
			std::cout << "It's working!\n";
		}


		camera->Update();


		for (size_t t = 0; t < 22; t++) {
			planets[t].angle += planets[t].angleSpeed;
			planets[t].angle2 += planets[t].angleSpeed2;
			Vector3 rot = {
			planets[t].joint.x + planets[t].radius * sin(Math::Radians(planets[t].angle)) * cos(Math::Radians(planets[t].angle2)),
			planets[t].joint.y + planets[t].radius * sin(Math::Radians(planets[t].angle)) * sin(Math::Radians(planets[t].angle2)),
			planets[t].joint.z + planets[t].radius * cos(Math::Radians(planets[t].angle)),
			
			};
			transform[t].SetPosition(rot); 
			transform[t].AdjustEulerRotation(Vector3(planets[t].angleSpeed2, planets[t].angleSpeed, 0));
			transform[t].Update();
		}

		planets[7].joint = transform[0].GetWorldPosition();
		planets[8].joint = transform[0].GetWorldPosition();
		planets[9].joint = transform[0].GetWorldPosition();
		planets[10].joint = transform[1].GetWorldPosition();
		planets[11].joint = transform[1].GetWorldPosition();
		planets[12].joint = transform[1].GetWorldPosition();
		planets[13].joint = transform[2].GetWorldPosition();
		planets[14].joint = transform[2].GetWorldPosition();
		planets[15].joint = transform[2].GetWorldPosition();
		planets[16].joint = transform[3].GetWorldPosition();
		planets[17].joint = transform[3].GetWorldPosition();
		planets[18].joint = transform[3].GetWorldPosition();
		planets[19].joint = transform[4].GetWorldPosition();
		planets[20].joint = transform[4].GetWorldPosition();
		planets[21].joint = transform[5].GetWorldPosition();


		for (size_t t = 0; t < 23; t++) {
			Components[t]->Update(camera->Proj().Transpose(), camera->View().Transpose(), transform[t].GetWorldMatrix().Transpose());
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
		for (size_t t = 0; t < 22; t++) {
			transform[t].SetPosition(Vector3(0, 0, 0));
		}
	

		transform[0].SetScale(Vector3(15.0f,15.0f,15.0f));
		transform[1].SetScale(Vector3(15.0f,15.0f,15.0f));
		transform[2].SetScale(Vector3(15.0f,15.0f,15.0f));
		transform[3].SetScale(Vector3(15.0f,15.0f,15.0f));
		transform[4].SetScale(Vector3(15.0f,15.0f,15.0f));	
		transform[5].SetScale(Vector3(15.0f,15.0f,15.0f));
		transform[6].SetScale(Vector3(30.0f,30.0f,30.0f));

		transform[22].SetPosition(Vector3(0, 0, 0));
		transform[22].SetScale(Vector3(10.0f, 10.0f, 10.0f));
		transform[22].SetEulerRotate(Vector3(0, 0, 0));


		for (size_t t = 7; t < 22; t++) {
			transform[t].SetPosition(Vector3(2.0f, 2.0f, 2.0f));
		}

		for (size_t t = 7; t < 22; t++) {
			planets[t].angleSpeed = rand_FloatRange(-0.5, 0.5);
			planets[t].angleSpeed2 = rand_FloatRange(-0.5, 0.5);
			planets[t].radius = rand_FloatRange(10, 20);
		}

	



		planets[0].angleSpeed = rand_FloatRange(-0.5, 0.5);
		planets[1].angleSpeed = rand_FloatRange(-0.5, 0.5);
		planets[2].angleSpeed = rand_FloatRange(-0.5, 0.5);
		planets[3].angleSpeed = rand_FloatRange(-0.5, 0.5);
		planets[4].angleSpeed = rand_FloatRange(-0.5, 0.5);
		planets[5].angleSpeed = rand_FloatRange(-0.5, 0.5);
		planets[6].angleSpeed = rand_FloatRange(-1, 1);
		planets[0].angleSpeed2 = rand_FloatRange(-0.5, 0.5);
		planets[1].angleSpeed2 = rand_FloatRange(-0.5, 0.5);
		planets[2].angleSpeed2 = rand_FloatRange(-0.5, 0.5);
		planets[3].angleSpeed2 = rand_FloatRange(-0.5, 0.5);
		planets[4].angleSpeed2 = rand_FloatRange(-0.5, 0.5);
		planets[5].angleSpeed2 = rand_FloatRange(-0.5, 0.5);
		planets[6].angleSpeed2 = rand_FloatRange(-1, 1);

		planets[0].radius = rand_FloatRange(30, 90);
		planets[1].radius = rand_FloatRange(30, 90);
		planets[2].radius = rand_FloatRange(30, 90);
		planets[3].radius = rand_FloatRange(30, 90);
		planets[4].radius = rand_FloatRange(30, 90);
		planets[5].radius = rand_FloatRange(30, 90);
		planets[6].radius = 0;


		system->DrawLine(Vector3(0, -200, 0), Vector3(0, 200, 0), Color(0, 255, 0, 255));
		system->DrawLine(Vector3(-200, 0, 0), Vector3(200, 0, 0), Color(255, 0, 0, 255));
		system->DrawLine(Vector3(0,0, -200), Vector3(0, 0, 200), Color(0, 0, 255, 255));
	}

