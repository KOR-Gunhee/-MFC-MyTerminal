
// SensorControllerDlg.h : ��� ����
//

#pragma once

#include "OScopeCtrl.h"
#include "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxcmn.h"
#include "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxwin.h"



#define SZ_TX_BUFF 1024

#define CONT_RES_MAXROW 5000

// CSensorControllerDlg ��ȭ ����
class CSensorControllerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CSensorControllerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	COScopeCtrl *_rtGraph;
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENSORCONTROLLER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnOpen();

public:
	CString str_Port_Name;
	CString m_strJigRxData;
	int i_Rx_Line_Count;
	int i_Index_Tx_Buff;
	int i_Packet_End_Class;
	BYTE ab8_Tx_Buff[SZ_TX_BUFF];
	CString str_ADC;
	int ADC_flag;
	double db_adc_val;
	CListCtrl m_List;
	int i_list;

	CString str_NO_Buff[CONT_RES_MAXROW];
	CString str_ADC_Buff[CONT_RES_MAXROW];
	CString str_MV_Buff[CONT_RES_MAXROW];

	CEdit m_edit_msg;
	

public:
	void On_Msg_Serial_Port(long port_num, long status);
	int Insert_Tx_Buff(BYTE data);
	int Convert_Tx_Element(CString str, char mode);
	int Convert_Tx_String(CString str);
	int Convert_Tx_String2(CString str);
	int Send_Buff(CString portname, void *data, DWORD size);
	int Send_Packet(void);
	void Wait(DWORD dwMillisecond);
	void ToSendCmd(CString cmd);
	void OnSaveContRes(FILE* fp);

	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnSend2();
	afx_msg void OnBnClickedBtnSend3();
	afx_msg void OnBnClickedBtnSend4();
};
