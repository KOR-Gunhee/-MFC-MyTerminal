
// SensorControllerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "SensorController.h"
#include "SensorControllerDlg.h"
#include "afxdialogex.h"

#include "Serial_Port.h"
//#include "..\..\..\include\Serial_Port.h"

#include <iostream>
#include <string.h>
#include <cstdlib>
#include <time.h>



#define MODE_ELEMENT_INT 0
#define MODE_ELEMENT_HEX 1
#define MODE_ELEMENT_STR 2


#define MODE_PACKET_END_NONE 0
#define MODE_PACKET_END_RETURN 1
#define MODE_PACKET_END_CHKSUM 2
#define MODE_PACKET_END_CRC16 3

#define SZ_TLB_COMBO_END_STR 4
const TCHAR* str_Combo_End[SZ_TLB_COMBO_END_STR] = { _T("없음"),_T("CR(\\r\\n)"),_T("체크섬(16)"),_T("CRC(16)") };

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSensorControllerDlg 대화 상자
const unsigned short crctable[256] = {
	0x0000,    0x1021,    0x2042,    0x3063,    0x4084,    0x50a5,    0x60c6,    0x70e7,
	0x8108,    0x9129,    0xa14a,    0xb16b,    0xc18c,    0xd1ad,    0xe1ce,    0xf1ef,
	0x1231,    0x0210,    0x3273,    0x2252,    0x52b5,    0x4294,    0x72f7,    0x62d6,
	0x9339,    0x8318,    0xb37b,    0xa35a,    0xd3bd,    0xc39c,    0xf3ff,    0xe3de,
	0x2462,    0x3443,    0x0420,    0x1401,    0x64e6,    0x74c7,    0x44a4,    0x5485,
	0xa56a,    0xb54b,    0x8528,    0x9509,    0xe5ee,    0xf5cf,    0xc5ac,    0xd58d,
	0x3653,    0x2672,    0x1611,    0x0630,    0x76d7,    0x66f6,    0x5695,    0x46b4,
	0xb75b,    0xa77a,    0x9719,    0x8738,    0xf7df,    0xe7fe,    0xd79d,    0xc7bc,
	0x48c4,    0x58e5,    0x6886,    0x78a7,    0x0840,    0x1861,    0x2802,    0x3823,
	0xc9cc,    0xd9ed,    0xe98e,    0xf9af,    0x8948,    0x9969,    0xa90a,    0xb92b,
	0x5af5,    0x4ad4,    0x7ab7,    0x6a96,    0x1a71,    0x0a50,    0x3a33,    0x2a12,
	0xdbfd,    0xcbdc,    0xfbbf,    0xeb9e,    0x9b79,    0x8b58,    0xbb3b,    0xab1a,
	0x6ca6,    0x7c87,    0x4ce4,    0x5cc5,    0x2c22,    0x3c03,    0x0c60,    0x1c41,
	0xedae,    0xfd8f,    0xcdec,    0xddcd,    0xad2a,    0xbd0b,    0x8d68,    0x9d49,
	0x7e97,    0x6eb6,    0x5ed5,    0x4ef4,    0x3e13,    0x2e32,    0x1e51,    0x0e70,
	0xff9f,    0xefbe,    0xdfdd,    0xcffc,    0xbf1b,    0xaf3a,    0x9f59,    0x8f78,
	0x9188,    0x81a9,    0xb1ca,    0xa1eb,    0xd10c,    0xc12d,    0xf14e,    0xe16f,
	0x1080,    0x00a1,    0x30c2,    0x20e3,    0x5004,    0x4025,    0x7046,    0x6067,
	0x83b9,    0x9398,    0xa3fb,    0xb3da,    0xc33d,    0xd31c,    0xe37f,    0xf35e,
	0x02b1,    0x1290,    0x22f3,    0x32d2,    0x4235,    0x5214,    0x6277,    0x7256,
	0xb5ea,    0xa5cb,    0x95a8,    0x8589,    0xf56e,    0xe54f,    0xd52c,    0xc50d,
	0x34e2,    0x24c3,    0x14a0,    0x0481,    0x7466,    0x6447,    0x5424,    0x4405,
	0xa7db,    0xb7fa,    0x8799,    0x97b8,    0xe75f,    0xf77e,    0xc71d,    0xd73c,
	0x26d3,    0x36f2,    0x0691,    0x16b0,    0x6657,    0x7676,    0x4615,    0x5634,
	0xd94c,    0xc96d,    0xf90e,    0xe92f,    0x99c8,    0x89e9,    0xb98a,    0xa9ab,
	0x5844,    0x4865,    0x7806,    0x6827,    0x18c0,    0x08e1,    0x3882,    0x28a3,
	0xcb7d,    0xdb5c,    0xeb3f,    0xfb1e,    0x8bf9,    0x9bd8,    0xabbb,    0xbb9a,
	0x4a75,    0x5a54,    0x6a37,    0x7a16,    0x0af1,    0x1ad0,    0x2ab3,    0x3a92,
	0xfd2e,    0xed0f,    0xdd6c,    0xcd4d,    0xbdaa,    0xad8b,    0x9de8,    0x8dc9,
	0x7c26,    0x6c07,    0x5c64,    0x4c45,    0x3ca2,    0x2c83,    0x1ce0,    0x0cc1,
	0xef1f,    0xff3e,    0xcf5d,    0xdf7c,    0xaf9b,    0xbfba,    0x8fd9,    0x9ff8,
	0x6e17,    0x7e36,    0x4e55,    0x5e74,    0x2e93,    0x3eb2,    0x0ed1,    0x1ef0
};
#define CRC16(CRC,VAL)  CRC=((CRC<<8)^crctable[((CRC>>8)^VAL)&0xff])&0xffff
#define CRC16_INIT(CRC) CRC=0xffff
#define CRC16_END(CRC)	CRC^=0x0000


CSensorControllerDlg *pMainDlg;

CSensorControllerDlg::CSensorControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENSORCONTROLLER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pMainDlg = this;
	str_Port_Name = _T("");
	i_Rx_Line_Count = 0;
	i_Index_Tx_Buff = 0;
	i_Packet_End_Class = 0;
	m_strJigRxData = "";
}

void CSensorControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG, m_edit_msg);
}

BEGIN_MESSAGE_MAP(CSensorControllerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CSensorControllerDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSensorControllerDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_SEND2, &CSensorControllerDlg::OnBnClickedBtnSend2)
	ON_BN_CLICKED(IDC_BTN_SEND3, &CSensorControllerDlg::OnBnClickedBtnSend3)
	ON_BN_CLICKED(IDC_BTN_SEND4, &CSensorControllerDlg::OnBnClickedBtnSend4)
END_MESSAGE_MAP()

void CALLBACK On_Msg_Serial_Port(long port_num, long status)
{
	if (pMainDlg->GetSafeHwnd() != NULL)pMainDlg->On_Msg_Serial_Port(port_num, status);
	return;

}
// CSensorControllerDlg 메시지 처리기

BOOL CSensorControllerDlg::OnInitDialog()
{
	int int00;
	TCHAR name_device[255];
	BYTE  value_device[255];
	long ret;
	CComboBox *pcombobox;
	DWORD size_name, size_value, type_value;

	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.

	pcombobox = (CComboBox *)GetDlgItem(IDC_COMBO_PORT);
	pcombobox->ResetContent();

	HKEY regkey00;
	if (RegOpenKey(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &regkey00) == ERROR_SUCCESS)
	{
		int00 = 0;
		do {
			size_name = 255;
			size_value = 255;
			type_value = REG_SZ;
			ret = RegEnumValue(regkey00, int00, name_device, &size_name, NULL, &type_value, value_device, &size_value);
			if (ret == ERROR_SUCCESS)
			{
				pcombobox->InsertString(int00, (TCHAR *)value_device);
			}
			int00++;
			TRACE(_T("name = %s,  val=%s\n"), name_device, value_device);
		} while (ret != ERROR_NO_MORE_ITEMS);
		RegCloseKey(regkey00);

	}

	if (pcombobox->GetCount()>0)
	{
		pcombobox->SetCurSel(0);
	}

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSensorControllerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSensorControllerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSensorControllerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1000) {
		double t = (double)GetTickCount() / 1000.;
		double value[3] = { cos(t), sin(t), tan(t) };

		double val00[1] = { (rand() % 3000)};

		//_rtGraph->AppendPoints(val00);
	}

	CDialog::OnTimer(nIDEvent);
}

void CSensorControllerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	delete _rtGraph;
}

//void CSensorControllerDlg::On_Msg_Serial_Port(long port_num, long status)
//{
//	CString str00, str01, str02, str_temp;
//	BYTE buff[1024 * sizeof(BYTE)];
//	int int00;
//	int read_size;
//	BYTE byte00;
//	CEdit *pedit;
//
//	switch (HIWORD(port_num))
//	{
//	case 0: //rx
//		if ((status&EV_RXCHAR) == EV_RXCHAR)
//		{
//
//			str00.Format(_T("COM%d"), port_num);
//			pedit = (CEdit *)GetDlgItem(IDC_EDIT_RX);
//			pedit->GetWindowText(str01);
//			if (str01.GetLength() >= 4096)
//			{
//				str01 = "";
//				str02 = "";
//			}
//			//pedit = (CEdit *)GetDlgItem(IDC_EDIT_RX_HEX);
//			//pedit->GetWindowText(str02);
//			//if (str02.GetLength() >= 4096)
//			//{
//			//	str01 = "";
//			//	str02 = "";
//			//}
//			do
//			{
//				read_size = Read_Serial_Port(str_Port_Name, buff, 1024);
//
//
//				for (int00 = 0;int00<read_size;int00++)
//				{
//					byte00 = buff[int00];
//					str_temp.Format(_T("[%02x]"), byte00);
//
//					m_strJigRxData += byte00;
//
//					/////////////////////////////TEST CODE//////////////////
//					if (byte00 == 'c'){ADC_flag = 5;}
//
//					if ((ADC_flag < 4) && (ADC_flag > 0))
//					{
//						str_ADC += byte00;
//						if (ADC_flag == 1)
//						{
//							i_adc_val = atoi(str_ADC);
//							double val00[1] = { i_adc_val };
//							_rtGraph->AppendPoints(val00);
//							str_ADC = "";
//						}
//					}
//					ADC_flag--;
//					////////////////////////////////////////////////////////////
//#if 1
//
//					if (i_Rx_Line_Count<16)
//					{
//						switch ((TCHAR)byte00)
//						{
//						case '\r':
//							//pedit->SetSel(-1, -1);
//						case '\n':
//							//str01=_T("");
//							str01 += _T("\r\n");
//							break;
//						default:
//							str01 += (TCHAR)buff[int00];						
//							break;
//						}
//						str02 += str_temp;
//						i_Rx_Line_Count++;
//					}
//					else
//					{
//						str01 += _T("\r\n");
//						str02 += _T("\r\n");
//						str01 += (TCHAR)buff[int00];
//						str02 += str_temp;
//						i_Rx_Line_Count = 0;
//					}
//#endif
//				}
//			} while (read_size>0);
//
//			pedit = (CEdit *)GetDlgItem(IDC_EDIT_RX);
////			pedit->SetWindowxText(str01);
//			pedit->SetWindowText(str_ADC);
//			pedit->LineScroll(pedit->GetLineCount());
//			//pedit = (CEdit *)GetDlgItem(IDC_EDIT_RX_HEX);
//			//pedit->SetWindowText(str02);
//			//pedit->LineScroll(pedit->GetLineCount());
//			//RECEIVE DATA	
//		}
//		break;
//	case 1: //tx
//		break;
//	}
//}

void CSensorControllerDlg::On_Msg_Serial_Port(long port_num, long status)
{
	CString str00, str01, str02, str_temp;
	BYTE buff[1024 * sizeof(BYTE)];
	int int00;
	int read_size;
	BYTE byte00;
	CEdit *pedit;
	CString str33;
//	char szHex[16];

	switch (HIWORD(port_num))
	{
	case 0: //rx
		if ((status&EV_RXCHAR) == EV_RXCHAR)
		{

			str00.Format(_T("COM%d"), port_num);
			pedit = (CEdit *)GetDlgItem(IDC_EDIT_RX);
			pedit->GetWindowText(str01);

			//pedit->GetWindowText(str33);

			if (str01.GetLength() >= 4096)
			{
				str01 = _T("");
				str02 = _T("");
			}
			//pedit = (CEdit *)GetDlgItem(IDC_EDIT_RX_HEX);
			//pedit->GetWindowText(str02);
			//if (str02.GetLength() >= 4096)
			//{
			//	str01 = "";

			//	str02 = "";
			//}
			do
			{
				read_size = Read_Serial_Port(str_Port_Name, buff, 1024);

				for (int00 = 0; int00<read_size; int00++)
				{
					byte00 = buff[int00];
					str_temp.Format(_T("[%02x]"), byte00);

					m_strJigRxData += byte00;
#if 1
					if (i_Rx_Line_Count<16)
					{
						switch ((TCHAR)byte00)
						{
						case _T('\r'):
							//pedit->SetSel(-1, -1);
						case _T('\n'):
							//str01=_T("");
							str01 += _T("\r\n");
							i_Rx_Line_Count = 0;
							break;
						default:
							str01 += (TCHAR)buff[int00];
							break;
						}
						str02 += str_temp;
						i_Rx_Line_Count++;
					}
					else
					{
						str01 += _T("\r\n");
						str02 += _T("\r\n");
						str01 += (TCHAR)buff[int00];
						str02 += str_temp;
						i_Rx_Line_Count = 0;
					}
#endif
				}
			} while (read_size>0);

			pedit = (CEdit *)GetDlgItem(IDC_EDIT_RX);
			pedit->SetWindowText(str01);
			
			//pedit->SetWindowText(str_ADC);
			pedit->LineScroll(pedit->GetLineCount());
			//pedit = (CEdit *)GetDlgItem(IDC_EDIT_RX_HEX);
			//pedit->SetWindowText(str02);
			//pedit->LineScroll(pedit->GetLineCount());
			//RECEIVE DATA	
		}
		break;
	case 1: //tx
		break;
	}
}

void CSensorControllerDlg::OnBnClickedBtnOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str00;

	CComboBox *pcombobox;
	pcombobox = (CComboBox *)GetDlgItem(IDC_COMBO_PORT);
	pcombobox->GetLBText(pcombobox->GetCurSel(), str00);

	COMMCONFIG st_com_config;
	memset(&st_com_config, 0, sizeof(COMMCONFIG));
	st_com_config.dwSize = sizeof(COMMCONFIG);
	st_com_config.dwProviderSubType = PST_RS232;
	st_com_config.dwProviderOffset = 0;
	st_com_config.dwProviderSize = 0;
	st_com_config.wVersion = 1;
	st_com_config.dcb.DCBlength = sizeof(DCB);
	if (CommConfigDialog(str00, NULL, &st_com_config))
	{
		if (str_Port_Name.IsEmpty() == FALSE)
		{
			if (str_Port_Name.Compare(str00) == 0)
			{
				Close_Serial_Port(str_Port_Name);
			}
			else
			{
				Free_Serial_Port(str_Port_Name);
				str_Port_Name = _T("");
				if (Create_Serial_Port(str00)>0)str_Port_Name = str00;
			}
		}
		else
		{
			if (Create_Serial_Port(str00)>0)str_Port_Name = str00;
		}

		Create_Serial_Port(str_Port_Name);
		Set_Callback_Serial_Port(str_Port_Name, ::On_Msg_Serial_Port);
		//Open_Serial_Port(str_Port_Name,st_com_config.dcb);
		Set_SW_Control_Serial_Port(str_Port_Name,
			st_com_config.dcb.fTXContinueOnXoff,
			st_com_config.dcb.fInX,
			st_com_config.dcb.fOutX,
			CONST_UNUSED, CONST_UNUSED, CONST_UNUSED, CONST_UNUSED);

		Set_HW_Control_Serial_Port(str_Port_Name, st_com_config.dcb.fDtrControl, st_com_config.dcb.fDsrSensitivity, st_com_config.dcb.fRtsControl);
		Open_Serial_Port(str_Port_Name,
			st_com_config.dcb.BaudRate,
			st_com_config.dcb.Parity,
			st_com_config.dcb.StopBits,
			st_com_config.dcb.ByteSize);
	}
}

int CSensorControllerDlg::Insert_Tx_Buff(BYTE data)
{
	if (i_Index_Tx_Buff<SZ_TX_BUFF)
	{
		ab8_Tx_Buff[i_Index_Tx_Buff++] = data;
		return 1;
	}
	return 0;
}

int CSensorControllerDlg::Convert_Tx_Element(CString str, char mode)
{

	int count;
	int len;
//	int int00;
//	char ch00;
	BYTE byte00;
	TCHAR tchar00;
	len = str.GetLength();
	if (len)
	{
		switch (mode)
		{

		case MODE_ELEMENT_HEX://hex;
			byte00 = 0;
			for (count = 0;count<len;count++)
			{
				tchar00 = str.GetAt(count);

				if ((tchar00 >= _T('0')) && (tchar00 <= _T('9')))
				{
					byte00 <<= 4;
					byte00 |= tchar00 - _T('0');
				}
				else if ((tchar00 >= _T('A')) && (tchar00 <= _T('F')))
				{
					byte00 <<= 4;
					byte00 |= tchar00 - _T('A') + 10;
				}
				else if ((tchar00 >= _T('a')) && (tchar00 <= _T('f')))
				{
					byte00 <<= 4;
					byte00 |= tchar00 - _T('a') + 10;
				}


			}
			Insert_Tx_Buff(byte00);
			break;
		case MODE_ELEMENT_STR://string;
			for (count = 0;count<len;count++)
			{
				Insert_Tx_Buff((BYTE)str.GetAt(count));
			}
			break;
		default://int
			byte00 = 0;
			for (count = 0;count<len;count++)
			{
				tchar00 = str.GetAt(count);

				if ((tchar00 >= _T('0')) && (tchar00 <= _T('9')))
				{
					byte00 *= 10;
					byte00 += tchar00 - _T('0');
				}



			}
			Insert_Tx_Buff(byte00);
			break;
		}
		return 1;
	}
	return 0;

}

int CSensorControllerDlg::Convert_Tx_String(CString str)
{
	CString str_temp;
	TCHAR tchar00;
	int intex;
	intex = 1;
	int char_mode = 2;
	//0=int,1=hex,2 = string, 
	i_Index_Tx_Buff = 0;
	str_temp = _T("");
	do {
		tchar00 = str.GetAt(intex);
		if (tchar00 == _T('\0'))break;
		intex++;
		switch (tchar00)
		{
		case _T('"'):
			Convert_Tx_Element(str_temp, char_mode);
			if (char_mode == MODE_ELEMENT_STR)char_mode = MODE_ELEMENT_INT;
			else char_mode = MODE_ELEMENT_STR;
			str_temp = _T("");
			break;
		case _T('+'):
		case _T(','):
			if (char_mode == MODE_ELEMENT_STR)
			{
				str_temp += tchar00;

				break;
			}
			Convert_Tx_Element(str_temp, char_mode);
			str_temp = _T("");
			char_mode = MODE_ELEMENT_INT;
			break;
		case _T('X'):
		case _T('x'):
			if (char_mode == MODE_ELEMENT_STR)
			{
				str_temp += tchar00;

				break;
			}
			char_mode = MODE_ELEMENT_HEX;
			str_temp = _T("");
			break;
		default:
			str_temp += tchar00;
			break;
		}
	} while (tchar00);
	if (str_temp.IsEmpty() == 0)Convert_Tx_Element(str_temp, char_mode);
	return 1;
}

int CSensorControllerDlg::Convert_Tx_String2(CString str)
{
	CString str_temp;
	TCHAR tchar00;
	int intex;
	intex = 1;
	int char_mode = 2;
	//0=int,1=hex,2 = string, 
	i_Index_Tx_Buff = 0;
	str_temp = _T("");
	do {
		tchar00 = str.GetAt(intex);
		if (tchar00 == _T('\0'))break;
		intex++;
		switch (tchar00)
		{
		case _T('"'):
			Convert_Tx_Element(str_temp, char_mode);
			if (char_mode == MODE_ELEMENT_STR)char_mode = MODE_ELEMENT_INT;
			else char_mode = MODE_ELEMENT_STR;
			str_temp = _T("");
			break;
		case _T('+'):
		case _T(','):
			if (char_mode == MODE_ELEMENT_STR)
			{
				str_temp += tchar00;

				break;
			}
			Convert_Tx_Element(str_temp, char_mode);
			str_temp = _T("");
			char_mode = MODE_ELEMENT_INT;
			break;
		case _T('X'):
		case _T('x'):
			if (char_mode == MODE_ELEMENT_STR)
			{
				str_temp += tchar00;

				break;
			}
			char_mode = MODE_ELEMENT_HEX;
			str_temp = _T("");
			break;
		default:
			str_temp += tchar00;
			break;
		}
	} while (tchar00);
	if (str_temp.IsEmpty() == 0)Convert_Tx_Element(str_temp, char_mode);
	return 1;
}

int CSensorControllerDlg::Send_Buff(CString portname, void *data, DWORD size)
{
	int ret;
	Wait_Write(portname, 5000);
	ret = Write_Serial_Port(portname, data, size);

	i_Index_Tx_Buff = 0;
	return ret;
}

int CSensorControllerDlg::Send_Packet(void)
{
	unsigned int crc;
	unsigned int checksum;
	int int00;
	switch (i_Packet_End_Class)
	{
	case MODE_PACKET_END_RETURN:
		Insert_Tx_Buff('\r');
		Insert_Tx_Buff('\n');
		break;
	case MODE_PACKET_END_CRC16:
		CRC16_INIT(crc);
		for (int00 = 0;int00<i_Index_Tx_Buff;int00++)
		{
			CRC16(crc, ab8_Tx_Buff[int00]);
		}
		CRC16_END(crc);
		Insert_Tx_Buff((BYTE)(crc >> 8));
		Insert_Tx_Buff((BYTE)crc);
		break;
	case MODE_PACKET_END_CHKSUM:
		checksum = 0;
		for (int00 = 0;int00<i_Index_Tx_Buff;int00++)
		{
			checksum += ab8_Tx_Buff[int00];
		}
		Insert_Tx_Buff((BYTE)(checksum >> 8));
		Insert_Tx_Buff((BYTE)checksum);
		break;

	}
	Send_Buff(str_Port_Name, ab8_Tx_Buff, i_Index_Tx_Buff);
	i_Index_Tx_Buff = 0;
	return 1;
}

void CSensorControllerDlg::Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}

void CSensorControllerDlg::OnBnClickedBtnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str00;
	CString str01;

	GetDlgItem(IDC_EDIT_MSG)->GetWindowText(str00);
	str01.Format("bIS:3FFFF %s\r", str00);
	//Convert_Tx_String2("bIS:3FFFF 0.1\r");
	//Send_Packet();
	//Wait(100);
	Convert_Tx_String2(str01);
	Send_Packet();
	Wait(100);
}

void CSensorControllerDlg::OnBnClickedBtnSend2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str00;
	GetDlgItem(IDC_EDIT_MSG)->GetWindowText(str00);
	//Convert_Tx_String2(str00);
	Convert_Tx_String2("bEN:3FFFF\r");
	Send_Packet();
	Wait(100);
}

void CSensorControllerDlg::OnBnClickedBtnSend3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str00;
	GetDlgItem(IDC_EDIT_MSG)->GetWindowText(str00);
	//Convert_Tx_String2(str00);
	Convert_Tx_String2("bDI:3FFFF\r");
	Send_Packet();
	Wait(100);
}


void CSensorControllerDlg::OnBnClickedBtnSend4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str00;
	GetDlgItem(IDC_EDIT_MSG)->GetWindowText(str00);
	//Convert_Tx_String2(str00);
	Convert_Tx_String2("bIM:3FFFF\r");
	Send_Packet();
	Wait(100);
}