
/***************************************** OBSTACLE DETECTION AND AVOIDANCE = "LOOKAHEAD" ****************************/

/****************************** IMPORTING C++, OpenCv, and Camera Explorer SDK LIBRARIES **************************/
#pragma once

#include <string.h>
#include <string>
#include <iomanip>
#include <iostream>


#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "pxcsensemanager.h"
#include "pxccapture.h"
#include "pxcmetadata.h"
#include "util_render.h"

#include <Windows.h> // to play sound in VS
#include <MMSystem.h>
/******************************************************************************************************************/

#define WIDTH 640 //define the width and height of the captured frame.
#define HEIGHT 480
bool paused = false; // boolean variable initialization.

enum ImageFormat {
	STREAM_TYPE_COLOR = 0,
	STREAM_TYPE_DEPTH = 1,
	STREAM_TYPE_IR = 2
};

namespace Obstacle {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Speech::Synthesis;
	using namespace cv;
	
	public ref class MyForm : public System::Windows::Forms::Form
	{
	
	private: Thread^ thread; // create object thread for parallel operation.









	
	public:
		MyForm(void){
	
			InitializeComponent();
		
			thread = gcnew Thread(gcnew ThreadStart(this, &MyForm::MyThreadFunc));
	
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button2;
	protected:
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::PictureBox^  image1;
	private: System::Windows::Forms::PictureBox^  image3;
	private: System::Windows::Forms::PictureBox^  image2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->image1 = (gcnew System::Windows::Forms::PictureBox());
			this->image3 = (gcnew System::Windows::Forms::PictureBox());
			this->image2 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image2))->BeginInit();
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->Location = System::Drawing::Point(521, 326);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(86, 35);
			this->button2->TabIndex = 7;
			this->button2->Text = L"Close";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::Color::Silver;
			this->button3->Enabled = false;
			this->button3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button3->Location = System::Drawing::Point(262, 326);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(80, 35);
			this->button3->TabIndex = 5;
			this->button3->Text = L"Pause";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::Lime;
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->ForeColor = System::Drawing::Color::Black;
			this->button1->Location = System::Drawing::Point(3, 326);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(84, 35);
			this->button1->TabIndex = 4;
			this->button1->Text = L"Start";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// image1
			// 
			this->image1->BackColor = System::Drawing::Color::White;
			this->image1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->image1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"image1.Image")));
			this->image1->Location = System::Drawing::Point(3, 23);
			this->image1->Name = L"image1";
			this->image1->Size = System::Drawing::Size(253, 297);
			this->image1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->image1->TabIndex = 2;
			this->image1->TabStop = false;
			// 
			// image3
			// 
			this->image3->BackColor = System::Drawing::Color::White;
			this->image3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->image3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"image3.Image")));
			this->image3->Location = System::Drawing::Point(521, 23);
			this->image3->Name = L"image3";
			this->image3->Size = System::Drawing::Size(255, 297);
			this->image3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->image3->TabIndex = 6;
			this->image3->TabStop = false;
			// 
			// image2
			// 
			this->image2->BackColor = System::Drawing::Color::White;
			this->image2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->image2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"image2.Image")));
			this->image2->Location = System::Drawing::Point(262, 23);
			this->image2->Name = L"image2";
			this->image2->Size = System::Drawing::Size(253, 297);
			this->image2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->image2->TabIndex = 3;
			this->image2->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Bodoni MT", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::Color::Green;
			this->label1->Location = System::Drawing::Point(3, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(95, 20);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Color Stream";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Bodoni MT", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::Color::Green;
			this->label2->Location = System::Drawing::Point(262, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(100, 20);
			this->label2->TabIndex = 9;
			this->label2->Text = L"Depth Stream";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Bodoni MT", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::Color::Green;
			this->label3->Location = System::Drawing::Point(521, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(121, 20);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Obstacle location";
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 3;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33333F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33334F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				33.33334F)));
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->image3, 2, 1);
			this->tableLayoutPanel1->Controls->Add(this->label3, 2, 0);
			this->tableLayoutPanel1->Controls->Add(this->button1, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->image2, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->label2, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->image1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->button3, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->button2, 2, 2);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 3;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(779, 373);
			this->tableLayoutPanel1->TabIndex = 11;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Ivory;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(779, 373);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->DoubleBuffered = true;
			this->ForeColor = System::Drawing::Color::Black;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MyForm";
			this->Text = L"Guide";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image2))->EndInit();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
  
/********************************* THE BACK BONE OF OUR WORK IS DEPICTED IN FUNCTION MyThreadFun() ***********************************/

	private: void MyThreadFunc()
	{
		// PXCSenseManager: organizes a pipeline by starting, stopping, and pausing the
		// operations of its various modalities.

			PXCSenseManager *psm = NULL; 
			psm = PXCSenseManager::CreateInstance(); // instance is created.

			SpeechSynthesizer^notify = gcnew SpeechSynthesizer(); // creation of object for voice notification/decision.

			// if instance is not created for the pipline or psm==NULL the following statement executed.				
		if (psm == NULL) {
			std::cout << "Unabel to create the PXCSenseManager" << std::endl;
			return;
			}

			// the activate functions that enables the camera to capture/ACQUSITION. 
			psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, WIDTH, HEIGHT);
			psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 0, 0);

			// Set the coordinate system, PXCSession = manages all of the modules of the SDK.
			PXCSession *session = psm->QuerySession();
			session->SetCoordinateSystem(PXCSession::COORDINATE_SYSTEM_REAR_OPENCV);

		if (psm->Init() != PXC_STATUS_NO_ERROR) { //if not, the camera is initialized.
				std::cout << "Unable to Init the PXCSenseManager" << std::endl;
				return;
			}

			PXCImage *colorIm, *depthIm; // create pointers for depth information and color information to access them separately.

			bool w = false, x = false, y = false, z = false;//boolean variable declaration for Decision rules.



	//.......... infinity loop for: capturing frames, applying proper image processing, setting rules for Decisions and display.....................//

		while (true) {

			//
			if (psm->AcquireFrame(true) < PXC_STATUS_NO_ERROR) {
				break;
			}

				// create a pointer that points both color and depth informations.
				PXCCapture::Sample *sample = psm->QuerySample();

				// Accessing only color image. it is less relevant for our objective. we gonna jump to depth information.
			if (sample) {
				if (sample->color) {
					colorIm = sample->color;
					cv::Mat colorMat,cool8;
					ConvertPXCImageToOpenCVMat(colorIm, &colorMat);
					DrawCVImageDetect(image1,colorMat);
				}
				// From now on, we focus only on DEPTH INFORMATION.


				// Accessing only depth image, converting it to MAT format, filtering noise, removeing outliers, divideing the image into blocks,
				//mean computation per block, decisions are made based on computed mean and finally demonstrations are set below.

			if (sample->depth) {

					depthIm = sample->depth;

					cv::Mat depthMat, dep2, medianFilter2, dep3(468, 640, CV_8UC1);// , dep;
							dep3 = 0;

					ConvertPXCImageToOpenCVMat(depthIm, &depthMat); // a function which convert the PXCIMAGE (ORIGINAL FORMAT, it is 16 bit) 
																	//to Mat format is called.
					
					//.................... NOISE ELLIMINATION = filtering.....................//

					medianBlur(depthMat, medianFilter2, 5);   

					medianFilter2.convertTo(dep2, CV_8UC1);	 // convert the 16 bit data to 8 bit data (which is suitable for display).
					//depthMat.convertTo(dep, CV_8UC1);//?
					DrawCVImageDetect(image2, dep2);		// displaying the filtered depth image.
					//imshow("filtered", dep2);//?
					//imshow("noisy", dep);//?
				//..................VARIABLE DECLARATIONS FOR MEAN COMPUTATION.....................................................................//

					float sumlc = 0,sumrc = 0, sumcc = 0; 
					float mlcenter, mccenter, mrcenter;
					int l = 1, c = 1, r = 1; // TO AVOID DIVIDING BY ZERO PROBLEM, WE INITIALIZE THESE VARIBLES AS 1, 
											//BECAUSE WE PROVED ONE PIXEL DIFFERENCE DOES NOT AFFECT THE SYSTEM PERFORMANCE;
					float n = 60, f = 210; // IT IS THE CAMERA RANGE; 60 cm =NEAR RANGE; 210 cm = far range.

					//OUTLIER REMOVAL

					for (int i = 0; i < 468; i++)     //height=rows =468
					{
						for (int j = 0; j < 640; j++) // width=columns, widthxheight or columsxrows = on camera sdk display.
						{
							if ((medianFilter2.at<unsigned short>(i,j)<600) || (medianFilter2.at<unsigned short>(i, j)>2100))
							{
								medianFilter2.at<unsigned short>(i, j) = 0;
							}
							
						}
						
					}

					// MEAN COMPUTATION

					for (int i = 0; i < 468; i++)
					{
						for (int j = 0; j < 213; j++)
						{

							sumlc = sumlc + (float)medianFilter2.at<unsigned short>(i, j);
							if (medianFilter2.at<unsigned short>(i, j) != 0)
							{
								l = l + 1;
							}
							sumcc = sumcc + (float)medianFilter2.at<unsigned short>(i, j+213);
							if (medianFilter2.at<unsigned short>(i, j + 213) != 0)
							{
								c = c + 1;
							}
							sumrc = sumrc + (float)medianFilter2.at<unsigned short>(i,j + 426);
							if (medianFilter2.at<unsigned short>(i, j + 426))
							{
								r = r + 1;
							}
							
						}

					}

							mlcenter = sumlc / (l);
							mccenter = sumcc / (c);
							mrcenter = sumrc / (r);
					
							// weighting the mean values found above. weighting factor = 0.1, which is based on our experiment.
							// and then casting to integer values. Casting also can be considered as weighting since it removes decimal values.

							mlcenter = (int)mlcenter / 10;
							mccenter = (int)mccenter / 10;
							mrcenter = (int)mrcenter / 10;


			/******************* FIRST LOCATE WHERE THE OBSTACLE IS, AND THEN DECISIONS ARE MADE IN WHICH DIRECTION THE USER HAS TO GO ****************************/
					
			
				// Decision while the Obstacle is on the left side of the user. decision, turn right.
					
				if ((((mlcenter<mrcenter) && (mlcenter <= mccenter)) ||
						((mrcenter == 0) && (mlcenter != 0))) && (w == false))
					{
						for (int i = 0; i < 156; i++)
						{
							for (int j = 0; j < 190; j++)
							{
								dep3.at<uchar>(i+156,j+23)= 255;
								
							}
						}
						DrawCVImageDetect(image3, dep3);
						notify->Speak("The Obstacle is on your left side please turn right");
						w = true;
						x = false;
						y = false;
						z = false;
					}

				// Decision while the Obstacle is on the right side of the user: decision, turn left.

					else if ((((mrcenter<mlcenter) && (mrcenter <= mccenter)) ||
						((mlcenter == 0) && ((mccenter != 0) || (mrcenter != 0)))) && (x == false))
					{
						for (int i = 0; i < 156; i++)
						{
							for (int j = 0; j < 190; j++)
							{
								dep3.at<uchar>(i + 156, j + 426) = 255;

							}
						}
						DrawCVImageDetect(image3, dep3);
						notify->Speak("The Obstacle is on your right side please turn left");
						w = false;
						x = true;
						y = false;
						z = false;
					}

				// Decision while the Obstacle is in front of the user: decision, either turn left or right.

					else if (((mccenter<mlcenter) && (mccenter<mrcenter)) && (y == false))
					{
						for (int i = 0; i < 156; i++)
						{
							for (int j = 0; j < 190; j++)
							{
								dep3.at<uchar>(i + 156, j + 213) = 255;

							}
						}
						DrawCVImageDetect(image3, dep3);
						notify->Speak("The Obstacle is in front please turn either left or right");
						w = false;
						x = false;
						y = true;
						z = false;
					}

			  // Decision while the Obstacle is on the left and right side of the user,
			  //and if the values are Out of camera range: decision, go ahead.

					else if ((((mrcenter == mlcenter) && (mrcenter == mccenter))
						|| ((mccenter == 0) && ((mlcenter != 0) && (mrcenter != 0)))
						|| ((mlcenter == mrcenter) && (mlcenter<mccenter))) && (z == false))
					{
						for (int i = 0; i < 156; i++)
						{
							for (int j = 0; j < 190; j++)
							{
								dep3.at<uchar>(i + 156, j + 23) = 255;
								dep3.at<uchar>(i + 156, j + 426) = 255;

							}
						}
						DrawCVImageDetect(image3, dep3);
						notify->Speak("The Obstacle is on your left and right sides please go ahead");
						w = false;
						x = false;
						y = false;
						z = true;

					}

				}
				//.................... end of " if (sample->depth) " statemnt or end of our core task............................//

			}
				//.................... end of infinity loop or WHILE loop........................................................//

			psm->ReleaseFrame(); //Release the frame for reading the next samples (color + depth).
		}
		psm->Release(); // closing; function to clean up
	}

	// a functin that converts the Original PXCImage format to Mat format( for simplicity of image processing).

	void ConvertPXCImageToOpenCVMat(PXCImage *inImg, Mat *outImg) {

					int cvDataType;
					int cvDataWidth;

					PXCImage::ImageData data; // stracture imageData
					inImg->AcquireAccess(PXCImage::ACCESS_READ, &data); // to access image from buffers.
					PXCImage::ImageInfo imgInfo = inImg->QueryInfo();

				switch (data.format) {
						// STREAM_TYPE_COLOR 
					case PXCImage::PIXEL_FORMAT_YUY2: //* YUY2 image  */
					case PXCImage::PIXEL_FORMAT_NV12: //* NV12 image */
						throw(0); // Not implemented
					case PXCImage::PIXEL_FORMAT_RGB32: //* BGRA layout on a little-endian machine */
						cvDataType = CV_8UC4;
						cvDataWidth = 4;
						break;
					case PXCImage::PIXEL_FORMAT_RGB24: //* BGR layout on a little-endian machine */
						cvDataType = CV_8UC3;
						cvDataWidth = 3;
						break;
					case PXCImage::PIXEL_FORMAT_Y8:  /* 8-Bit Gray Image, or IR 8-bit */
						cvDataType = CV_8U;
						cvDataWidth = 1;
						break;

						//* STREAM_TYPE_DEPTH 
					case PXCImage::PIXEL_FORMAT_DEPTH: /* 16-bit unsigned integer with precision mm. */
					case PXCImage::PIXEL_FORMAT_DEPTH_RAW: /* 16-bit unsigned integer with device specific precision (call device->QueryDepthUnit()) */
						cvDataType = CV_16U;
						cvDataWidth = 2;
						break;
					case PXCImage::PIXEL_FORMAT_DEPTH_F32: /* 32-bit float-point with precision mm. */
						cvDataType = CV_32F;
						cvDataWidth = 4;
						break;

						/* STREAM_TYPE_IR */
					case PXCImage::PIXEL_FORMAT_Y16:          /* 16-Bit Gray Image */
						cvDataType = CV_16U;
						cvDataWidth = 2;
						break;
					case PXCImage::PIXEL_FORMAT_Y8_IR_RELATIVE:    /* Relative IR Image */
						cvDataType = CV_8U;
						cvDataWidth = 1;
						break;
		              }

			// suppose that no other planes, no other pitches except planes[0] and pitches[0].
			if (data.planes[1] != NULL) throw(0); // not implemented
												  // suppose that no sub pixel padding needed
			if (data.pitches[0] % cvDataWidth != 0) throw(0); // not implemented

			outImg->create(imgInfo.height, data.pitches[0] / cvDataWidth, cvDataType);
			memcpy(outImg->data, data.planes[0], imgInfo.height*imgInfo.width*cvDataWidth*sizeof(pxcBYTE));
			inImg->ReleaseAccess(&data); // When the access is completed, use the ReleaseAccess() function to release the lock.
	
		}

	// a functin that displays: the original color image, the filtered depth image 
	// and the final processed depth image, based on the decision. 

	 void DrawCVImageDetect(System::Windows::Forms::PictureBox^ PBox, cv::Mat& colorImage)
			 {
				 System::Drawing::Graphics^ graphics = PBox->CreateGraphics();
				 System::IntPtr ptr(colorImage.ptr());
				 System::Drawing::Bitmap^ b;
             switch (colorImage.type())
				 {
				 case CV_8UC3: // non-grayscale images are correctly displayed here,3 channel unsigned char.
					 b = gcnew System::Drawing::Bitmap(colorImage.cols, colorImage.rows, colorImage.step,
						 System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
					 break;
				 case CV_8UC4: // non-grayscale images are correctly displayed here, 
					 b = gcnew System::Drawing::Bitmap(colorImage.cols, colorImage.rows, colorImage.step,
						 System::Drawing::Imaging::PixelFormat::Format32bppRgb, ptr);
					 break;
				 case CV_8UC1: // grayscale images are incorrectly displayed here 
					 b = gcnew System::Drawing::Bitmap(colorImage.cols, colorImage.rows, colorImage.step,
						 System::Drawing::Imaging::PixelFormat::Format8bppIndexed, ptr);
					 break;
				 default:
					 // error message
					 break;
				 }
				 System::Drawing::RectangleF rect(0, 0, (float)PBox->Width, (float)PBox->Height);
				 graphics->DrawImage(b, rect);

	}
  
	 // the following 5 functions are for controlling buttons of our graphycal interface.

	private: void shutDown()
	{
				
		thread->Abort();		
		
	}

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

	thread->Start();
	this->button1->Enabled = false;
	this->button3->Enabled = true;
		}

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 shutDown();
				 Application::Exit();
	 }

	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {

	if (paused == false)
	{
		this->button3->Text = L"Resume";
		this->button2->Enabled = false;
		thread->Suspend();
		paused = true;
	}
	else
		{ 
		this->button3->Text = L"Pause";
		this->button2->Enabled = true;
		thread->Resume();
		paused = false;
		
		}
	
	}

	
	private: System::Void MyForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
	shutDown();
	}
	};
}
