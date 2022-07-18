#include "Window.h"

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Armageddon::Window* Armageddon::Window::WindowInstance = nullptr;

Armageddon::Window::Window(int height, int width, std::wstring title, std::wstring wclass) : w_height(height), w_width(width), w_title(title), w_class(wclass)
{
	WindowInstance = this;

	if (Armageddon::RendererAPI::m_CurrentAPI == Armageddon::RendererAPI::API::Vulkan) {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		GLFWwindow* window = glfwCreateWindow(height, width, "Armageddon Editor", NULL, NULL);
		VkSurfaceKHR surface;
		VkInstance instance;

		//Initialisation of vulkan 

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; //Define struct type
		appInfo.pApplicationName = "Armageddon 2.0";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Armageddon 2.0";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		//This next struct is not optional and tells the Vulkan driver which global extensions and validation layers we want to use.
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // use gltf exetensions for vulkan 
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = glfwExtensionCount; // Need extension to handle every platforms
		createInfo.ppEnabledExtensionNames = glfwExtensions;// Need extension to handle every platforms
		createInfo.enabledLayerCount = 0; // reste obscur pour l'instant

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		if (result != VK_SUCCESS)
		{
			Armageddon::Log::GetLogger()->error("Error while creating Vulkan Instance : {0}", result);
			exit(-1);
		}
		VkResult err = glfwCreateWindowSurface(instance, window, NULL, &surface);

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		if (deviceCount <= 0)
		{
			Armageddon::Log::GetLogger()->error("No device Found : {0}", deviceCount);
			exit(-1);
		}
		Armageddon::Log::GetLogger()->info("Number of devices : {0}", deviceCount);
		VkPhysicalDeviceProperties deviceProperty;
		vkGetPhysicalDeviceProperties(devices[0], &deviceProperty);
		Armageddon::Log::GetLogger()->info("Device : {0}", deviceProperty.deviceName);
		Armageddon::Log::GetLogger()->info("VendorID : {0}", deviceProperty.vendorID);
		Armageddon::Log::GetLogger()->info("DeviceID : {0}", deviceProperty.deviceID);
		Armageddon::Log::GetLogger()->info("DeviceType : {0}", deviceProperty.deviceType);

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
		vkDestroyInstance(instance, nullptr);
	}
	else {
		Win32Window(width, height, title, wclass);
	}
}

void Armageddon::Window::Win32Window(int width, int height, std::wstring title, std::wstring wclass)
{
	this->ModuleInstance = (HINSTANCE)GetModuleHandle(NULL);

	this->RegisterWindowClass();


	this->WindowHandle = CreateWindowEx(
		0,										// Optional window styles.
		this->w_class.c_str(),							// Window class
		this->w_title.c_str(),    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style	
		CW_USEDEFAULT, CW_USEDEFAULT,
		width,
		height, // Size and position
		NULL,       // Parent window    
		NULL,       // Menu
		this->ModuleInstance,						// Instance handle
		this								 // Additional application data
	);

	if (this->WindowHandle == NULL)
	{
		Armageddon::Log::GetLogger()->trace("CreateWindowEx a échoué");

	}

	if (!m_Renderer.Init(this->WindowHandle, this->w_height, this->w_width))
		{
			Armageddon::Log::GetLogger()->trace("Erreur lors de la crétation du renderer");

		}
		//	m_PhysEngine.init();


	ShowWindow(this->WindowHandle, SW_MAXIMIZE);
	SetForegroundWindow(this->WindowHandle);
	SetFocus(this->WindowHandle);
}

void Armageddon::Window::GLFWindow(int width, int height, std::wstring title, std::wstring wclass)
{
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Armageddon::Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wparam, lparam))
	{
		return true;
	}

	switch (uMsg)
	{

	case WM_SIZE:
	{
		float width = LOWORD(lparam);
		float height = HIWORD(lparam);
		if (wparam != SIZE_MINIMIZED /*&& graphics.Getdevice() != NULL*/)
		{
			m_Renderer.ResizeBuffer(width, height);
		}

		this->w_height = height;
		this->w_width = width;
	
		return 0;
	}
	case WM_CHAR:
	{


		unsigned char letter = static_cast<unsigned char>(wparam); // on récupère la lettre via le param Wparam
		m_KeyBoard.OnChar(letter);


		return 0;
	}
	case WM_KEYDOWN:
	{


		unsigned char keytype = static_cast<unsigned char>(wparam);
		KeyBoardCallBack(keytype);
		m_KeyBoard.OnKeyPressed(keytype);


		return 0;
	}

	case WM_KEYUP:
	{

		unsigned char keytype = static_cast<unsigned char>(wparam);
		m_KeyBoard.OnKeyReleased(keytype);

		return 0;

	}
	case WM_MOUSEMOVE:
	{
		float x = LOWORD(lparam);
		float y = HIWORD(lparam);

		MouseCallBack(MouseEvent::MEventType::Move, x - m_old_mouse_posX, y - m_old_mouse_posY);

		m_old_mouse_posX = x;
		m_old_mouse_posY = y;
		return 0;
	}
	case WM_LBUTTONDOWN:
	{

		int x = LOWORD(lparam);												 
		int y = HIWORD(lparam);
		MouseCallBack(MouseEvent::MEventType::Lpress, x, y);

		return 0;
	}
	case WM_RBUTTONDOWN:
	{

		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		MouseCallBack(MouseEvent::MEventType::Rpress, x, y);

		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		MouseCallBack(MouseEvent::MEventType::Mpress, x, y);

		return 0;
	}
	case WM_LBUTTONUP:
	{

		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		MouseCallBack(MouseEvent::MEventType::Lrelease, x, y);

		return 0;
	}
	case WM_RBUTTONUP:
	{

		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		MouseCallBack(MouseEvent::MEventType::Rrelease, x, y);

		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		MouseCallBack(MouseEvent::MEventType::Mpress, x, y);

		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		{
			MouseCallBack(MouseEvent::MEventType::Wheelup, x, y);


		}
		else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{

			MouseCallBack(MouseEvent::MEventType::Wheeldown, x, y);

		}
		break;
	}
	/*case WM_INPUT:
	{

		UINT DataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &DataSize, sizeof(RAWINPUTHEADER));
		if (DataSize > 0)
		{
			std::unique_ptr<BYTE[]> raw_data = std::make_unique<BYTE[]>(DataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, raw_data.get(), &DataSize, sizeof(RAWINPUTHEADER)) == DataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(raw_data.get());
				TCHAR pszDest[1000];
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					//mouse.onRawMouseMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				//	MouseCallBack(MouseEvent::MEventType::RAW_MOVE);
				}
			}
		}
		return DefWindowProc(hwnd, uMsg, wparam, lparam);
	}*/


	default:
		return DefWindowProc(hwnd, uMsg, wparam, lparam);

	}
}

bool Armageddon::Window::ProcessMessage()
{

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	m_Renderer.RenderFrame();

	if (PeekMessage(&msg,
		this->WindowHandle,
		0,
		0,
		PM_REMOVE))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->WindowHandle))
		{
			this->WindowHandle = NULL;
			UnregisterClass(this->w_class.c_str(), this->ModuleInstance);
			return false;
		}
	}
	return true;
}
std::wstring Armageddon::Window::OpenDialog(const char* filter /*= "All Files (*.*)\0*.*\0"*/, HWND owner /*= NULL*/)
 {
	/*Armageddon::Log::GetLogger()->trace(filter);
	OPENFILENAME ofn;
	wchar_t fileName[255] = L"";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter =L"All Files (*.*)\0*.*\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = { 0 };



	if (GetOpenFileName(&ofn))
		fileNameStr = fileName;
		*/
	std::wstring fileNameStr;

	return fileNameStr;
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{

	if (hwnd == NULL)
	{
		Armageddon::Log::GetLogger()->error("HWND est null");
	}
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		return 0;
	}

	default:
		Armageddon::Window* const ptrWindow = reinterpret_cast<Armageddon::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (ptrWindow == NULL)
		{
			Armageddon::Log::GetLogger()->error("ERREUR CRITIQUE  PTRWINDOW  EST NULL");
		}
		return ptrWindow->WindowProc(hwnd, uMsg, wparam, lparam);

	}
}


LRESULT CALLBACK CustomWindowProcSetup(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		Armageddon::Log::GetLogger()->trace("WINDOW CREATE EVENT.");

		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
		Armageddon::Window* ptrWindow = reinterpret_cast<Armageddon::Window*>(pCreate->lpCreateParams);
		if (ptrWindow == nullptr)
		{
			Armageddon::Log::GetLogger()->error("Critical Error: Pointer to window is null.");

			exit(-1);
		}
			/*RAWINPUTDEVICE Rid;

			Rid.usUsagePage = 0x01;
			Rid.usUsage = 0x02;
			Rid.dwFlags = 0;
			Rid.hwndTarget = nullptr;


			if (RegisterRawInputDevices(&Rid, 1, sizeof(Rid)) == FALSE) {
				GetLastError();
				Armageddon::Log::GetLogger()->trace("ERROR WHEN REGISTERING RAW INPUT DEVICE");
			}*/

		

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptrWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));



		return ptrWindow->WindowProc(hwnd, uMsg, wparam, lparam);

	}

	default:
		return DefWindowProc(hwnd, uMsg, wparam, lparam);
	}

}


void Armageddon::Window::RegisterWindowClass()
{
	WNDCLASSEX wc; //Our Window Class (This has to be filled before our window can be created) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
	wc.style = CS_HREDRAW | CS_VREDRAW; //Flags [Redraw on width/height change from resiz/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
	wc.lpfnWndProc = CustomWindowProcSetup; //Pointer to Window Proc function for handling messages from this window
	wc.cbClsExtra = 0; //# of extra bytes to allocate following the window-class structure. We are not currently using this.
	wc.cbWndExtra = 0; //# of extra bytes to allocate following the window instance. We are not currently using this.
	wc.hInstance = this->ModuleInstance; //Handle to the instance that contains the Window Procedure
	wc.hIcon = NULL;   //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
	wc.hIconSm = NULL; //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
	wc.hbrBackground = NULL; //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
	wc.lpszMenuName = NULL; //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
	wc.lpszClassName = this->w_class.c_str(); //Pointer to null terminated string of our class name for this window.
	wc.cbSize = sizeof(WNDCLASSEX); //Need to fill in the size of our struct for cbSize
	RegisterClassEx(&wc); // Register the class so that it is usable.

}



