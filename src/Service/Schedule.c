//
//Copyright(C), 2007-2008, XUPT Univ.  
//������ţ�TTMS_UC_04	TTMS_UC_06
//File name: Schedule.c
//Description : �ݳ��ƻ�ҵ���߼���ͷ�ļ�
//Author:   XUPT
//Version:  v.1
//Date: 	2015��4��22��
//

#include "Schedule.h"
#include "../Persistence/Schedule_Persist.h"
#include <stdlib.h>
#include <assert.h>

//
//��ʶ����TTMS_SCU_Schedule_Srv_FetchByPlay
//����������int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id)��
//�������ܣ����ݲ����еľ�ĿID�ţ�������þ�Ŀ��ص��ݳ��ƻ���
//����˵����listΪschedule_list_t����ָ�룬��ʾ�����������ݳ��ƻ���Ϣ�ĵ�����ͷָ�룻play_idΪ���ͣ���ʾ�����ĿID�š�
//�� �� ֵ�����ͣ���ʾ���ϻ�ȡ�������ݳ��ƻ���������
//
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id){

    int rtn = Schedule_Perst_SelectByPlay(list,play_id);
    return rtn;
}

//��ʶ����TTMS_SCU_Schedule_Srv_Add
//����������int Schedule_Srv_Add(schedule_t* data);
//�������ܣ�������data��Ϊʵ�ε��ó־û���洢���ݳ��ƻ������������־û��㺯���ķ���ֵ���ݸ�����㺯����
//����˵����dataΪschedule_t����ָ�룬��ʾ�����ӵ����ݳ��ƻ����ݵĵ�ַ��
//����ֵ�����ͷ���1��ʾ�������ݳ��ƻ��ɹ�������0��ʾ�������ݳ��ƻ�ʧ�ܡ�
int Schedule_Srv_Add(schedule_t* data)
{
    int rtn = Schedule_Perst_Insert(data);
	return rtn;
}

//
//��ʶ����TTMS_SCU_Schedule_Srv_Mod
//����������int Schedule_Srv_Modify(const schedule_t* data);
//�������ܣ��ò���data�е��ݳ��ƻ�������Ϊʵ�Σ�ͨ�����ó־û��㺯�����޸��ļ��ļ��м�¼�ľɵ��ݳ��ƻ���Ϣ��
//����˵����dataΪschedule_t����ָ�룬��ʾ���޸ĵ��ݳ��ƻ���Ϣ��
//����ֵ�����ͣ�����1��ʾ�޸��ݳ��ƻ��ɹ�����1��ʾ�޸��ݳ��ƻ�ʧ�ܡ�
//
int Schedule_Srv_Modify(const schedule_t* data)
{
	int rtn = Schedule_Perst_Update(data);
    return rtn;
}

//
//��ʶ����TTMS_SCU_Schedule_Srv_DelByID
//����������int Schedule_Srv_DeleteByID(int id);
//�������ܣ����ݲ���id��¼���ݳ��ƻ�ID�ţ����ó־û��㺯��ɾ����Ӧ�ݳ��ƻ���
//����˵����idΪ���ͣ���ʾ��ɾ�����ݳ��ƻ�ID�š�
//����ֵ�����ͣ�����1��ʾɾ���ݳ��ƻ��ɹ�����1��ʾɾ���ݳ��ƻ�ʧ�ܡ�
//
int Schedule_Srv_DeleteByID(int id)
{
	int rtn = Schedule_Perst_DeleteByID(id);
	return rtn;
}

int Schedule_Srv_FetchAll(schedule_list_t list)
{
	return Schedule_Perst_SelectAll(list);
}






