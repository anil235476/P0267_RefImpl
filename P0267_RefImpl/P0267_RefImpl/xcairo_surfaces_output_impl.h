#pragma once
#include "xcairo_surfaces_impl.h"

namespace std::experimental::io2d {
	inline namespace v1 {
		namespace _Cairo {
			// output surface functions

			template<class GraphicsMath>
			inline typename _Cairo_graphics_surfaces<GraphicsMath>::surfaces::output_surface_data_type _Cairo_graphics_surfaces<GraphicsMath>::surfaces::create_output_surface(int preferredWidth, int preferredHeight, io2d::format preferredFormat, io2d::scaling scl, io2d::refresh_style rr, float fps) {
				output_surface_data_type result;
				_Display_surface_data_type& data = result.data;
				data.display_dimensions.x(preferredWidth);
				data.display_dimensions.y(preferredHeight);
				data.rr = rr;
				data.refresh_fps = fps;
				data.scl = scl;
				data.back_buffer.format = preferredFormat;
				data.back_buffer.dimensions.x(preferredWidth);
				data.back_buffer.dimensions.y(preferredHeight);
#if defined(USE_XLIB)
				data.display = move(unique_ptr<Display, decltype(&XCloseDisplay)>(XOpenDisplay(nullptr), &XCloseDisplay));
				if (data.display == nullptr) {
					throw ::std::system_error(::std::make_error_code(::std::errc::io_error));
				}
				data.visual = DefaultVisual(data.display.get(), DefaultScreen(data.display.get()));
				data.wmDeleteWndw = XInternAtom(data.display.get(), "WM_DELETE_WINDOW", False);
#endif
				return result;
			}
			template<class GraphicsMath>
			inline typename _Cairo_graphics_surfaces<GraphicsMath>::surfaces::output_surface_data_type _Cairo_graphics_surfaces<GraphicsMath>::surfaces::create_output_surface(int preferredWidth, int preferredHeight, io2d::format preferredFormat, error_code& ec, io2d::scaling scl, io2d::refresh_style rr, float fps) noexcept {
				output_surface_data_type result;
				_Display_surface_data_type& data = result.data;
				data.display_dimensions.x(preferredWidth);
				data.display_dimensions.y(preferredHeight);
				data.rr = rr;
				data.refresh_style = fps;
				data.scl = scl;
				data.back_buffer.format = preferredFormat;
				data.back_buffer.dimensions.x(preferredWidth);
				data.back_buffer.dimensions.y(preferredHeight);
#if defined(USE_XLIB)
				data.display = move(unique_ptr<Display, decltype(&XCloseDisplay)>(XOpenDisplay(nullptr), &XCloseDisplay));
				if (data.display == nullptr) {
					ec = ::std::make_error_code(::std::errc::io_error);
					return output_surface_data_type{};
				}
				data.visual = DefaultVisual(data.display.get(), DefaultScreen(data.display.get()));
				data.wmDeleteWndw = XInternAtom(data.display.get(), "WM_DELETE_WINDOW", False);
#endif
				ec.clear();
				return result;
			}
			template<class GraphicsMath>
			inline typename _Cairo_graphics_surfaces<GraphicsMath>::surfaces::output_surface_data_type _Cairo_graphics_surfaces<GraphicsMath>::surfaces::create_output_surface(int preferredWidth, int preferredHeight, io2d::format preferredFormat, int preferredDisplayWidth, int preferredDisplayHeight, io2d::scaling scl, io2d::refresh_style rr, float fps) {
				output_surface_data_type result;
				_Display_surface_data_type& data = result.data;
				data.display_dimensions.x(preferredDisplayWidth);
				data.display_dimensions.y(preferredDisplayHeight);
				data.rr = rr;
				data.refresh_style = fps;
				data.scl = scl;
				data.back_buffer.format = preferredFormat;
				data.back_buffer.dimensions.x(preferredWidth);
				data.back_buffer.dimensions.y(preferredHeight);
#if defined(USE_XLIB)
				data.display = move(unique_ptr<Display, decltype(&XCloseDisplay)>(XOpenDisplay(nullptr), &XCloseDisplay));
				if (data.display == nullptr) {
					throw ::std::system_error(::std::make_error_code(::std::errc::io_error));
				}
				data.visual = DefaultVisual(data.display.get(), DefaultScreen(data.display.get()));
				data.wmDeleteWndw = XInternAtom(data.display.get(), "WM_DELETE_WINDOW", False);
#endif
				return result;
			}
			template <class GraphicsMath>
			inline typename _Cairo_graphics_surfaces<GraphicsMath>::surfaces::output_surface_data_type _Cairo_graphics_surfaces<GraphicsMath>::surfaces::create_output_surface(int preferredWidth, int preferredHeight, io2d::format preferredFormat, int preferredDisplayWidth, int preferredDisplayHeight, error_code& ec, io2d::scaling scl, io2d::refresh_style rr, float fps) noexcept {
				output_surface_data_type result;
				_Display_surface_data_type& data = result.data;
				data.display_dimensions.x(preferredDisplayWidth);
				data.display_dimensions.y(preferredDisplayHeight);
				data.rr = rr;
				data.refresh_style = fps;
				data.scl = scl;
				data.back_buffer.format = preferredFormat;
				data.back_buffer.dimensions.x(preferredWidth);
				data.back_buffer.dimensions.y(preferredHeight);
#if defined(USE_XLIB)
				data.display = move(unique_ptr<Display, decltype(&XCloseDisplay)>(XOpenDisplay(nullptr), &XCloseDisplay));
				if (data.display == nullptr) {
					ec = ::std::make_error_code(::std::errc::io_error);
					return output_surface_data_type{};
				}
				data.visual = DefaultVisual(data.display.get(), DefaultScreen(data.display.get()));
				data.wmDeleteWndw = XInternAtom(data.display.get(), "WM_DELETE_WINDOW", False);
#endif
				ec.clear();
				return result;
			}
			template <class GraphicsMath>
			inline typename _Cairo_graphics_surfaces<GraphicsMath>::surfaces::output_surface_data_type _Cairo_graphics_surfaces<GraphicsMath>::surfaces::move_output_surface(output_surface_data_type&& data) noexcept {
				data.data.back_buffer = ::std::move(move_image_surface(::std::move(data.data.back_buffer)));
				return data;
			}
			template <class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::destroy(output_surface_data_type& data) noexcept {
				destroy(data.data.back_buffer);
			}
#if defined(_WIN32) || defined(_WIN64)
			template<class GraphicsMath>
			inline int _Cairo_graphics_surfaces<GraphicsMath>::surfaces::begin_show(output_surface_data_type& osd, basic_output_surface<_Cairo_graphics_surfaces<GraphicsMath>>* instance, basic_output_surface<_Cairo_graphics_surfaces<GraphicsMath>>& sfc) {
				_RegisterWindowClass();

				_Display_surface_data_type& data = osd.data;
				RECT rc;
				rc.top = 0;
				rc.left = 0;
				rc.right = data.display_dimensions.x();
				rc.bottom = data.display_dimensions.y();

				// Adjust the window size for correct device size
				if (!AdjustWindowRect(&rc, (WS_OVERLAPPEDWINDOW | WS_VISIBLE), FALSE)) {
					_Throw_system_error_for_GetLastError(GetLastError(), "Failed call to AdjustWindowRect in cairo_display_surface::cairo_display_surface(int, int, format, int, int, scaling).");
				}

				long lwidth = rc.right - rc.left;
				long lheight = rc.bottom - rc.top;

				long lleft = 10;
				long ltop = 10;

				//#if defined(UNICODE)
				const wchar_t* winTitle = L"";
				//#else
				//const char* winTitle = "";
				//#endif
				// Create an instance of the window
				data.hwnd = CreateWindowEx(
					static_cast<DWORD>(0),				// extended style
					_Refimpl_window_class_name,			// class name
					winTitle,							// instance title
					(WS_OVERLAPPEDWINDOW | WS_VISIBLE),	// window style
					lleft, ltop,						// initial x, y
					lwidth,								// initial width
					lheight,							// initial height
					static_cast<HWND>(nullptr),			// handle to parent
					static_cast<HMENU>(nullptr),		// handle to menu
					static_cast<HINSTANCE>(nullptr),	// instance of this application
					static_cast<LPVOID>(nullptr));		// extra creation parms

				if (data.hwnd == nullptr) {
					_Throw_system_error_for_GetLastError(GetLastError(), "Failed call to CreateWindowEx.");
				}

				SetLastError(ERROR_SUCCESS);
				if (SetWindowLongPtrW(data.hwnd, 0, reinterpret_cast<LONG_PTR>(instance)) == 0) {
					// SetWindowLongPtr is weird in terms of how it fails. See its documentation. Hence this weird check.
					DWORD lastError = GetLastError();
					if (lastError != ERROR_SUCCESS) {
						_Throw_system_error_for_GetLastError(lastError, "Failed call to SetWindowLongPtrW(HWND, int, LONG_PTR)");
					}
				}

				data.hasOwnDC = true;
				data.hdc = GetDC(data.hwnd);

				_Create_display_surface_and_context<GraphicsMath>(data);

				data._Default_letterbox_brush = basic_brush<_Cairo_graphics_surfaces>(rgba_color::black);
				data._Letterbox_brush = data._Default_letterbox_brush;

				data.back_buffer = ::std::move(create_image_surface(data.back_buffer.format, data.back_buffer.dimensions.x(), data.back_buffer.dimensions.y()));

				// Initially display the window
				ShowWindow(data.hwnd, SW_SHOWNORMAL);
				UpdateWindow(data.hwnd);

				MSG msg{};
				msg.message = WM_NULL;
				//			if (_Display_surface.native_handle()._Draw_fn == nullptr) {
				//				throw system_error(make_error_code(errc::operation_not_supported));
				//			}
				data.elapsed_draw_time = 0.0f;
#ifdef _IO2D_WIN32FRAMERATE
				auto previousTime = ::std::chrono::steady_clock::now();
				long long int elapsedDrawNanoseconds = 0LL;
				int updateTitleCounter = -1;
				deque<::std::chrono::nanoseconds> elapsedNanoseconds(static_cast<size_t>(30), ::std::chrono::nanoseconds(33'333'333));
#endif
				while (msg.message != WM_QUIT) {
#ifdef _IO2D_WIN32FRAMERATE
					auto currentTime = ::std::chrono::steady_clock::now();
					auto elapsedTimeIncrement = static_cast<float>(::std::chrono::duration_cast<::std::chrono::nanoseconds>(currentTime - previousTime).count());
					data.elapsed_draw_time += elapsedTimeIncrement;
					elapsedDrawNanoseconds += ::std::chrono::duration_cast<::std::chrono::nanoseconds>(currentTime - previousTime).count();
					previousTime = currentTime;
#endif
					if (!PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
						RECT clientRect;
						GetClientRect(data.hwnd, &clientRect);
						if (clientRect.right - clientRect.left != data.display_dimensions.x() || clientRect.bottom - clientRect.top != data.display_dimensions.y()) {
							// If there is a size mismatch we skip painting and resize the window instead.
							display_dimensions(osd, data.display_dimensions);
							if (data.display_dimensions != data.back_buffer.dimensions) {
								if (osd.size_change_callback != nullptr) {
									osd.size_change_callback(sfc);
								}
							}
							continue;
						}
						else {
							bool redraw = true;
							if (data.rr == io2d::refresh_style::as_needed) {
								redraw = data.redraw_required;
								data.redraw_required = false;
							}

							const auto desiredElapsed = 1'000'000'000.0F / data.refresh_fps;
#ifdef _IO2D_WIN32FRAMERATE
							const long long desiredElapsedNanoseconds = static_cast<long long>(1'000'000'000.00F / data.refresh_fps);
#endif
							if (data.rr == io2d::refresh_style::fixed) {
								// desiredElapsed is the amount of time, in nanoseconds, that must have passed before we should redraw.
								redraw = data.elapsed_draw_time >= desiredElapsed;
							}
							if (redraw) {
								// Run user draw function:
								osd.draw_callback(sfc);
								_Render_to_native_surface(osd, sfc);
#ifdef _IO2D_WIN32FRAMERATE
								elapsedNanoseconds.pop_front();
								elapsedNanoseconds.push_back(chrono::nanoseconds(elapsedDrawNanoseconds));
#endif
								if (data.rr == experimental::io2d::refresh_style::fixed) {
									while (data.elapsed_draw_time >= desiredElapsed) {
										data.elapsed_draw_time -= desiredElapsed;
									}
#ifdef _IO2D_WIN32FRAMERATE
									while (elapsedDrawNanoseconds >= desiredElapsedNanoseconds) {
										elapsedDrawNanoseconds -= desiredElapsedNanoseconds;
									}
#endif
								}
								else {
									data.elapsed_draw_time = 0.0F;
#ifdef _IO2D_WIN32FRAMERATE
									elapsedDrawNanoseconds = 0LL;
#endif
								}
							}
						}
					}
					else {
						if (msg.message != WM_QUIT) {
							TranslateMessage(&msg);
							DispatchMessage(&msg);

							if (msg.message == WM_PAINT) {
								const auto desiredElapsed = 1'000'000'000.0F / data.refresh_fps;
#ifdef _IO2D_WIN32FRAMERATE
								elapsedNanoseconds.pop_front();
								elapsedNanoseconds.push_back(chrono::nanoseconds(elapsedDrawNanoseconds));

								const long long desiredElapsedNanoseconds = static_cast<long long>(1'000'000'000.00F / data.refresh_fps);
#endif
								if (data.rr == io2d::refresh_style::fixed) {
									while (data.elapsed_draw_time >= desiredElapsed) {
										data.elapsed_draw_time -= desiredElapsed;
									}
#ifdef _IO2D_WIN32FRAMERATE
									while (elapsedDrawNanoseconds >= desiredElapsedNanoseconds) {
										elapsedDrawNanoseconds -= desiredElapsedNanoseconds;
									}
#endif
								}
								else {
									data.elapsed_draw_time = 0.0F;
#ifdef _IO2D_WIN32FRAMERATE
									elapsedDrawNanoseconds = 0LL;
#endif
								}
							}
						}
					}

#ifdef _IO2D_WIN32FRAMERATE
					if (updateTitleCounter == -1) {
						SetWindowText(msg.hwnd, L"FPS: ");
					}
					updateTitleCounter++;
					if (updateTitleCounter == 50 && msg.hwnd == 0) {
						updateTitleCounter--;
					}
					if (updateTitleCounter == 50 && msg.hwnd != 0) {
						unsigned long long sumNanoElapsed = 0LL;
						for (auto iter = begin(elapsedNanoseconds), last = end(elapsedNanoseconds); iter != last; iter++) {
							const auto val = *iter;
							sumNanoElapsed += static_cast<unsigned long long>(val.count());
						}
						auto avgNanoFrameTime = (sumNanoElapsed / elapsedNanoseconds.size());
						const auto fpsNano = 1'000'000'000.0F / avgNanoFrameTime;
						wstringstream fpsStr;
						fpsStr << "FPS: " << fixed << setprecision(5) << fpsNano;
						wstring fpsStr_Str;
						fpsStr_Str = fpsStr.str();
						const wchar_t* fpsStrWindowText = fpsStr_Str.c_str();
						if (!SetWindowText(msg.hwnd, fpsStrWindowText)) {
							_Throw_system_error_for_GetLastError(GetLastError(), "Failed call to SetWindowText.");
						}
						updateTitleCounter = 0;
					}
#endif
				}
				data.elapsed_draw_time = 0.0F;
				return static_cast<int>(msg.wParam);
			}


			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::end_show(output_surface_data_type& /*data*/) {
				PostQuitMessage(0);
			}

			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::display_dimensions(output_surface_data_type& data, const basic_display_point<GraphicsMath>& val) {
				data.data.display_dimensions = val;
				if (data.data.hwnd != 0) {
					// Change window size then call _Ds_display_dimensions, I think.
					RECT clientRect;
					RECT windowRect;
					GetWindowRect(data.data.hwnd, &windowRect);
					GetClientRect(data.data.hwnd, &clientRect);
					auto crWidth = clientRect.right - clientRect.left;
					auto crHeight = clientRect.bottom - clientRect.top;
					auto wrWidth = windowRect.right - windowRect.left;
					auto wrHeight = windowRect.bottom - windowRect.top;
					auto displayWidth = val.x();
					auto displayHeight = val.y();
					if (crWidth != displayWidth || crHeight != displayHeight) {
						auto width = std::max((wrWidth - crWidth) + 1L, (displayWidth - crWidth) + wrWidth);
						auto height = std::max((wrHeight - crHeight) + 1L, (displayHeight - crHeight) + wrHeight);
						// Resize the window.
						if (!SetWindowPos(data.data.hwnd, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS)) {
							_Throw_system_error_for_GetLastError(GetLastError(), "Failed call to SetWindowPos.");
						}

						if (!PostMessageW(data.data.hwnd, WM_PAINT, static_cast<WPARAM>(0), static_cast<LPARAM>(0))) {
							_Throw_system_error_for_GetLastError(GetLastError(), "Failed call to PostMessageW.");
						}
					}
				}
				//_Ds_display_dimensions<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, val);
				_Create_display_surface_and_context<GraphicsMath>(data.data);
				data.data.redraw_required = true;
			}

#elif defined(USE_XLIB)

			Bool _X11_if_xev_pred(::Display* display, ::XEvent* xev, XPointer arg);

			template<class GraphicsMath>
			inline int _Cairo_graphics_surfaces<GraphicsMath>::surfaces::begin_show(output_surface_data_type& osd, basic_output_surface<_Cairo_graphics_surfaces<GraphicsMath>>* instance, basic_output_surface<_Cairo_graphics_surfaces<GraphicsMath>>& sfc) {
				_Display_surface_data_type& data = osd.data;
				Display* display = data.display.get();
				int screenNumber = DefaultScreen(display);
				int x = 0;
				int y = 0;
				unsigned int borderWidth = 4;
				data.wndw = XCreateSimpleWindow(display, RootWindow(display, screenNumber), x, y, static_cast<unsigned int>(data.display_dimensions.x()), static_cast<unsigned int>(data.display_dimensions.y()), borderWidth, WhitePixel(display, screenNumber), BlackPixel(display, screenNumber));
				XSelectInput(display, data.wndw, ExposureMask | StructureNotifyMask);
				XSetWMProtocols(display, data.wndw, &data.wmDeleteWndw, 1);
				XMapWindow(display, data.wndw);
				_Create_display_surface_and_context<GraphicsMath>(data);

				data._Default_letterbox_brush = basic_brush<_Cairo_graphics_surfaces>(rgba_color::black);
				data._Letterbox_brush = data._Default_letterbox_brush;

				data.back_buffer = ::std::move(create_image_surface(data.back_buffer.format, data.back_buffer.dimensions.x(), data.back_buffer.dimensions.y()));

				bool exit = false;
				XEvent xev;

				auto previousTime = ::std::chrono::steady_clock::now();
				data.elapsed_draw_time = 0.0F;
				while (!exit) {
					auto currentTime = ::std::chrono::steady_clock::now();
					auto elapsedTimeIncrement = static_cast<float>(::std::chrono::duration_cast<::std::chrono::nanoseconds>(currentTime - previousTime).count());
					data.elapsed_draw_time += elapsedTimeIncrement;
					previousTime = currentTime;
					while (XCheckIfEvent(data.display.get(), &xev, &_X11_if_xev_pred, reinterpret_cast<XPointer>(&osd))) {
						switch (xev.type) {
							// ExposureMask events:
						case Expose:
						{
							if (!data.can_draw && data.wndw != None) {
								_Create_display_surface_and_context<GraphicsMath>(data);
							}
							assert(data.display_surface != nullptr && data.display_context != nullptr);
							data.can_draw = true;
							if (osd.draw_callback != nullptr) {
								if (data.auto_clear) {
									_Ds_clear<_Cairo_graphics_surfaces<GraphicsMath>>(data);
								}
								osd.draw_callback(sfc);
							}
							else {
								throw system_error(make_error_code(errc::operation_not_supported));
							}
							_Render_to_native_surface(osd, sfc);

							data.elapsed_draw_time = 0.0F;
							//if (_Refresh_rate == experimental::io2d::refresh_style::fixed) {
							//	_Elapsed_draw_time -= elapsedTimeIncrement;
							//}
							//else {
							//	_Elapsed_draw_time = 0.0F;
							//}
						} break;
						// StructureNotifyMask events:
						case CirculateNotify:
						{
						} break;
						case ConfigureNotify:
						{
							bool resized = false;
							if (xev.xconfigure.width != data.display_dimensions.x()) {
								data.display_dimensions.x(xev.xconfigure.width);
								resized = true;
							}
							if (xev.xconfigure.height != data.display_dimensions.y()) {
								data.display_dimensions.y(xev.xconfigure.height);
								resized = true;
							}
							if (resized) {
								cairo_xlib_surface_set_size(data.display_surface.get(), static_cast<double>(data.display_dimensions.x()), static_cast<double>(data.display_dimensions.y()));
								if (osd.size_change_callback != nullptr) {
									osd.size_change_callback(sfc);
								}
							}
						} break;
						case DestroyNotify:
						{
							data.wndw = None;
							data.can_draw = false;
							data.display_context.reset();
							data.display_surface.reset();
							exit = true;
						} break;
						case GravityNotify:
						{
						} break;
						case MapNotify:
						{
							// After the window is mapped, and Expose event will be generated once the window is ACTUALLY ready to be drawn to. As such we do nothing here.
						} break;
						case ReparentNotify:
						{
						} break;
						case UnmapNotify:
						{
							// The window still exists, it has just been unmapped.
							data.can_draw = false;
							data.display_context.reset();
							data.display_surface.reset();
						} break;
						// Might get them even though they are unrequested events (see http://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#Event_Masks ):
						case GraphicsExpose:
						{
							if (data.can_draw) {
								if (osd.draw_callback != nullptr) {
									if (data.auto_clear) {
										_Ds_clear<_Cairo_graphics_surfaces<GraphicsMath>>(data);
									}
									osd.draw_callback(sfc);
								}
								else {
									throw system_error(make_error_code(errc::operation_not_supported));
								}
								_Render_to_native_surface(osd, sfc);

								data.elapsed_draw_time = 0.0F;
							}
						} break;
						case NoExpose:
						{
						} break;
						// Unmasked events
						case ClientMessage:
						{
							if (xev.xclient.format == 32 && static_cast<Atom>(xev.xclient.data.l[0]) == data.wmDeleteWndw) {
								data.can_draw = false;
								data.display_context.reset();
								data.display_surface.reset();
								XDestroyWindow(data.display.get(), data.wndw);
								data.wndw = None;
								exit = true;
							}
							else {
								stringstream clientMsgStr;
								clientMsgStr << "ClientMessage event type '" << xev.xclient.message_type << "' for unknown event type";
								auto atomName = XGetAtomName(data.display.get(), xev.xclient.message_type);
								if (atomName != nullptr) {
									try {
										clientMsgStr << " (" << atomName << ")";
									}
									catch (...) {
										XFree(atomName);
										throw;
									}
									XFree(atomName);
								}
								clientMsgStr << ". Format is '" << xev.xclient.format << "' and first value is '";
								switch (xev.xclient.format) {
								case 8:
								{
									clientMsgStr << to_string(static_cast<int>(xev.xclient.data.b[0])).c_str();
								} break;
								case 16:
								{
									clientMsgStr << to_string(xev.xclient.data.s[0]).c_str();
								} break;
								case 32:
								{
									clientMsgStr << to_string(xev.xclient.data.l[0]).c_str();
								} break;
								default:
								{
									assert("Unexpected format." && false);
									clientMsgStr << "(unexpected format)";
								} break;
								}
								clientMsgStr << "'.";
								auto es = clientMsgStr.str().c_str();
								cerr << es << endl;
							}
						} break;
						case MappingNotify:
						{
						} break;
						case SelectionClear:
						{
						} break;
						case SelectionNotify:
						{
						} break;
						case SelectionRequest:
						{
						} break;
						default:
						{
							stringstream errorString;
							errorString << "Unexpected xev.type. Value is '" << xev.type << "'.";
							cerr << errorString.str().c_str();
							assert(xev.type >= 64 && xev.type <= 127);
						} break;
						}
					}
					if (data.can_draw) {
						bool redraw = true;
						if (data.rr == io2d::refresh_style::as_needed) {
							redraw = data.redraw_required;
							data.redraw_required = false;
						}

						auto desiredElapsed = 1'000'000'000.0f / data.refresh_fps;

						if (data.rr == io2d::refresh_style::fixed) {
							// desiredElapsed is the amount of time, in nanoseconds, that must have passed before we should redraw.
							redraw = data.elapsed_draw_time >= desiredElapsed;
						}
						if (redraw) {
							// Run user draw function:
							if (osd.draw_callback != nullptr) {
								if (data.auto_clear) {
									_Ds_clear<_Cairo_graphics_surfaces<GraphicsMath>>(data);
								}
								osd.draw_callback(sfc);
							}
							else {
								throw system_error(make_error_code(errc::operation_not_supported));
							}
							_Render_to_native_surface(osd, sfc);
							if (data.rr == io2d::refresh_style::fixed) {
								while (data.elapsed_draw_time >= desiredElapsed) {
									data.elapsed_draw_time -= desiredElapsed;
								}
							}
							else {
								data.elapsed_draw_time = 0.0f;
							}
						}
					}
				}
				data.elapsed_draw_time = 0.0F;
				return 0;
			}

			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::end_show(output_surface_data_type& osd) {
				XDestroyWindow(osd.data.display.get(), osd.data.wndw);
			}

			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::display_dimensions(output_surface_data_type& osd, const basic_display_point<GraphicsMath>& val) {
				Window rootWindow{};
				int x{};
				int y{};
				unsigned int width = 0;
				unsigned int height = 0;
				unsigned int borderWidth{};
				unsigned int depth{};
				auto status = XGetGeometry(osd.data.display.get(), osd.data.wndw, &rootWindow, &x, &y, &width, &height, &borderWidth, &depth);
				if (status == 0) {
					_Throw_if_failed_cairo_status_t(CAIRO_STATUS_INVALID_STATUS);
				}
				if (width != static_cast<unsigned int>(osd.data.display_dimensions.x()) || height != static_cast<unsigned int>(osd.data.display_dimensions.y())) {
					XWindowChanges xwc{};
					xwc.width = osd.data.display_dimensions.x();
					xwc.height = osd.data.display_dimensions.y();
					XConfigureWindow(osd.data.display.get(), osd.data.wndw, CWWidth | CWHeight, &xwc);
				}
			}
#endif
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::refresh_style(output_surface_data_type& data, io2d::refresh_style val) {
				data.data.rr = val;
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::desired_frame_rate(output_surface_data_type& data, float val) {
				const float oneFramePerHour = 1.0f / (60.0f * 60.0f); // If you need a lower framerate than this, use as_needed and control the refresh by writing a timer that will trigger a refresh at your desired interval.
				const float maxFPS = 120.0f; // It's unlikely to find a display output that operates higher than this.
				data.data.refresh_fps = ::std::min(::std::max(val, oneFramePerHour), maxFPS);
			}
			template<class GraphicsMath>
			inline io2d::refresh_style _Cairo_graphics_surfaces<GraphicsMath>::surfaces::refresh_style(const output_surface_data_type& data) noexcept {
				return data.data.rr;
			}
			template<class GraphicsMath>
			inline float _Cairo_graphics_surfaces<GraphicsMath>::surfaces::desired_frame_rate(const output_surface_data_type& data) noexcept {
				return data.data.refresh_fps;
			}

			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::flush(output_surface_data_type& data) {
				cairo_surface_flush(data.data.back_buffer.surface.get());
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::flush(output_surface_data_type& data, error_code& ec) noexcept {
				cairo_surface_flush(data.data.back_buffer.surface.get());
				ec.clear();
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::mark_dirty(output_surface_data_type& data) {
				cairo_surface_mark_dirty(data.data.back_buffer.surface.get());
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::mark_dirty(output_surface_data_type& data, error_code& ec) noexcept {
				cairo_surface_mark_dirty(data.data.back_buffer.surface.get());
				ec.clear();
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::mark_dirty(output_surface_data_type& data, const basic_bounding_box<GraphicsMath>& extents) {
				cairo_surface_mark_dirty_rectangle(data.data.back_buffer.surface.get(), _Float_to_int(extents.x()), _Float_to_int(extents.y()), _Float_to_int(extents.width()), _Float_to_int(extents.height()));
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::mark_dirty(output_surface_data_type& data, const basic_bounding_box<GraphicsMath>& extents, error_code& ec) noexcept {
				cairo_surface_mark_dirty_rectangle(data.data.back_buffer.surface.get(), _Float_to_int(extents.x()), _Float_to_int(extents.y()), _Float_to_int(extents.width()), _Float_to_int(extents.height()));
				ec.clear();
			}

			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::clear(output_surface_data_type& data) {
				_Ds_clear<_Cairo_graphics_surfaces<GraphicsMath>>(data.data);
			}
			template <class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::paint(output_surface_data_type& data, const basic_brush<_Graphics_surfaces_type>& b, const basic_brush_props<_Graphics_surfaces_type>& bp, const basic_render_props<_Graphics_surfaces_type>& rp, const basic_clip_props<_Graphics_surfaces_type>& cl) {
				_Ds_paint<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, b, bp, rp, cl);
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::stroke(output_surface_data_type& data, const basic_brush<_Graphics_surfaces_type>& b, const basic_interpreted_path<_Graphics_surfaces_type>& ip, const basic_brush_props<_Graphics_surfaces_type>& bp, const basic_stroke_props<_Graphics_surfaces_type>& sp, const basic_dashes<_Graphics_surfaces_type>& d, const basic_render_props<_Graphics_surfaces_type>& rp, const basic_clip_props<_Graphics_surfaces_type>& cl) {
				_Ds_stroke<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, b, ip, bp, sp, d, rp, cl);
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::fill(output_surface_data_type& data, const basic_brush<_Graphics_surfaces_type>& b, const basic_interpreted_path<_Graphics_surfaces_type>& ip, const basic_brush_props<_Graphics_surfaces_type>& bp, const basic_render_props<_Graphics_surfaces_type>& rp, const basic_clip_props<_Graphics_surfaces_type>& cl) {
				_Ds_fill<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, b, ip, bp, rp, cl);
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::mask(output_surface_data_type& data, const basic_brush<_Graphics_surfaces_type>& b, const basic_brush<_Graphics_surfaces_type>& mb, const basic_brush_props<_Graphics_surfaces_type>& bp, const basic_mask_props<_Graphics_surfaces_type>& mp, const basic_render_props<_Graphics_surfaces_type>& rp, const basic_clip_props<_Graphics_surfaces_type>& cl) {
				_Ds_mask<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, b, mb, bp, mp, rp, cl);
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::draw_callback(output_surface_data_type& data, function<void(basic_output_surface<_Graphics_surfaces_type>&)> fn) {
				data.draw_callback = fn;
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::size_change_callback(output_surface_data_type& data, function<void(basic_output_surface<_Graphics_surfaces_type>&)> fn) {
				data.size_change_callback = fn;
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::user_scaling_callback(output_surface_data_type& data, function<basic_bounding_box<GraphicsMath>(const basic_output_surface<_Graphics_surfaces_type>&, bool&)> fn) {
				data.user_scaling_callback = fn;
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::dimensions(output_surface_data_type& data, const basic_display_point<GraphicsMath>& val) {
				_Ds_dimensions<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, val);
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::scaling(output_surface_data_type& data, io2d::scaling val) {
				_Ds_scaling<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, val);
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::letterbox_brush(output_surface_data_type& data, const optional<basic_brush<_Graphics_surfaces_type>>& val, const optional<basic_brush_props<_Graphics_surfaces_type>>& bp) noexcept {
				_Ds_letterbox_brush<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, val, bp);
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::letterbox_brush_props(output_surface_data_type& data, const basic_brush_props<_Graphics_surfaces_type>& val) {
				_Ds_letterbox_brush_props<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, val);
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::auto_clear(output_surface_data_type& data, bool val) {
				_Ds_auto_clear<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, val);
			}
			template<class GraphicsMath>
			inline void _Cairo_graphics_surfaces<GraphicsMath>::surfaces::redraw_required(output_surface_data_type& data, bool val) {
				_Ds_redraw_required<_Cairo_graphics_surfaces<GraphicsMath>>(data.data, val);
			}
			template<class GraphicsMath>
			inline io2d::format _Cairo_graphics_surfaces<GraphicsMath>::surfaces::format(const output_surface_data_type& data) noexcept {
				return _Ds_format<_Cairo_graphics_surfaces<GraphicsMath>>(data.data);
			}
			template<class GraphicsMath>
			inline basic_display_point<GraphicsMath> _Cairo_graphics_surfaces<GraphicsMath>::surfaces::dimensions(const output_surface_data_type& data) noexcept {
				return _Ds_dimensions<_Cairo_graphics_surfaces<GraphicsMath>>(data.data);
			}
			template<class GraphicsMath>
			inline basic_display_point<GraphicsMath> _Cairo_graphics_surfaces<GraphicsMath>::surfaces::display_dimensions(const output_surface_data_type& data) noexcept {
				return _Ds_display_dimensions<_Cairo_graphics_surfaces<GraphicsMath>>(data.data);
			}
			template<class GraphicsMath>
			inline io2d::scaling _Cairo_graphics_surfaces<GraphicsMath>::surfaces::scaling(output_surface_data_type& data) noexcept {
				return _Ds_scaling<_Cairo_graphics_surfaces<GraphicsMath>>(data.data);
			}
			template<class GraphicsMath>
			inline optional<basic_brush<_Cairo_graphics_surfaces<GraphicsMath>>> _Cairo_graphics_surfaces<GraphicsMath>::surfaces::letterbox_brush(const output_surface_data_type& data) noexcept {
				return _Ds_letterbox_brush<_Cairo_graphics_surfaces<GraphicsMath>>(data.data);
			}
			template<class GraphicsMath>
			inline basic_brush_props<_Cairo_graphics_surfaces<GraphicsMath>> _Cairo_graphics_surfaces<GraphicsMath>::surfaces::letterbox_brush_props(const output_surface_data_type& data) noexcept {
				return _Ds_letterbox_brush_props<_Cairo_graphics_surfaces<GraphicsMath>>(data.data);
			}
			template<class GraphicsMath>
			inline bool _Cairo_graphics_surfaces<GraphicsMath>::surfaces::auto_clear(const output_surface_data_type& data) noexcept {
				return _Ds_auto_clear<_Cairo_graphics_surfaces<GraphicsMath>>(data.data);
			}
			template<class GraphicsMath>
			inline bool _Cairo_graphics_surfaces<GraphicsMath>::surfaces::redraw_required(const output_surface_data_type& data) noexcept {
				return _Ds_redraw_required<_Cairo_graphics_surfaces<GraphicsMath>>(data.data);
			}
		}
	}
}
