#include "../include/usrmgr.h"

pClient head;

void InitLink() {                               //创建一个头结点
    head = (pClient) malloc(sizeof(Client));
    if (head == NULL)
        ERR_EXIT("malloc");
    head->next = NULL;
}

pClient GetHeadNode() {                         //返回头指针
    return head;
}

void AddClient(pClient client) {                //将一个指针指向的用户结构体添加到用户链表，头插法
    client->next = head->next;
    head->next = client;                        //第一个用户，是最新连接的用户
}

void RemoveClient(int fd) {                     //删除通过该套接口描述字连接的用户
    pClient p = head->next;
    pClient pre = head;
    while (p != NULL) {
        if (p->conn == fd) {
            pre->next = p->next;
            free(p);
            break;
        }
        pre = p;
        p = p->next;
    }
}

pClient FindClient(int fd) {                    //查找通过该套接口描述字连接的用户，返回用户结构体指针
    if (0 == fd)
        return NULL;
    pClient p = head->next;
    while (p != NULL) {
        if (fd == p->conn)
            break;
        p = p->next;
    }
    return p;
}

int CountConn() {                               //统计当前连接用户总量
    int count = 0;
    pClient p = head->next;
    while (p != NULL) {
        p = p->next;
        ++count;
    }
    return count;
}

void ClearClient() {                            //清空用户空间
    pClient p = head->next;
    while (p != NULL) {
        head->next = p->next;
        free(p);
        p = head->next;
    }
    free(head);
}

//void CheckConnection(){
//    	pClient p = head->next;
//    	while (p != NULL){
//		if (send(p->conn, "", sizeof(""), 0) == SOCKET_ERROR){
//			close(p->conn);
//			RemoveClient(p->conn);
//		}
//		p = p->next;
//	}
//}

