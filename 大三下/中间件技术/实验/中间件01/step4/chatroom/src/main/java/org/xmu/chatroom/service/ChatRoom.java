package org.xmu.chatroom.service;

import lombok.AllArgsConstructor;
import lombok.Data;
import org.apache.activemq.ActiveMQConnectionFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.xmu.chatroom.SseEmitterServer;

import javax.jms.*;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


@Data
public class ChatRoom {
    private Connection connection;
    private Session session;
    private Map<String, User> userMap;
    private Map<String, Topic> groupMap;
    private static Logger logger = LoggerFactory.getLogger(ChatRoom.class);

    public ChatRoom() {
        userMap = new HashMap<>();
        groupMap = new HashMap<>();
        var connectionFactory = new ActiveMQConnectionFactory("admin", "admin", "tcp://localhost:61616");
        try {
            connection = connectionFactory.createConnection();
            connection.start();
            session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);
        } catch (JMSException exception) {
            exception.printStackTrace();
        }
    }

    public void createUser(String uname) throws JMSException {
        var queue = session.createQueue(uname);
        var consumer = session.createConsumer(queue);
        var user = new User(uname, queue, null, consumer, new ArrayList<>());
        consumer.setMessageListener(user::onMessage);
        userMap.put(uname, user);
    }

    @Data
    @AllArgsConstructor
    public class User {
        private String uname;
        private Destination destination;
        private MessageProducer producer;
        private MessageConsumer consumer;
        private List<MessageConsumer> topicConsumers;

        public void connect(String receiverUname) throws JMSException {
            var receiver = userMap.get(receiverUname);
            producer = ChatRoom.this.session.createProducer(receiver.destination);
            logger.info("{}开启与{}的聊天。", uname, receiverUname);
        }

        public void connectWithGroup(String groupName) throws JMSException {
            var receiver = groupMap.get(groupName);
            producer = ChatRoom.this.session.createProducer(receiver);
            logger.info("{}开启群聊{}中的聊天。", uname, groupName);
        }

        public void sendMessage(String content) throws JMSException {
            var formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
            var message = String.format("[%s] %s: %s", formatter.format(LocalDateTime.now()), uname, content);
            var textMessage = session.createTextMessage(message);
            SseEmitterServer.sendMessage(uname, "已发送消息：" + textMessage.getText());
            producer.send(textMessage);
        }

        public void onMessage(Message message) {
            var textMessage = (TextMessage)message;
            try {
                SseEmitterServer.sendMessage(uname, "接收到消息：" + textMessage.getText());
            } catch (JMSException e) {
                e.printStackTrace();
            }
        }

        public void enterGroup(String groupName) throws JMSException {
            if (groupMap.get(groupName) == null) {
                groupMap.put(groupName, session.createTopic(groupName));
            }
            var topic = groupMap.get(groupName);
            var consumer = session.createConsumer(topic);
            consumer.setMessageListener(this::onMessage);
            this.topicConsumers.add(consumer);
        }

    }
}
