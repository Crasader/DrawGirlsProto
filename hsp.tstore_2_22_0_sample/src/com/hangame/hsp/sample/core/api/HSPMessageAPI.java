
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPMessage;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;
import com.hangame.hsp.sample.core.UiController;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class HSPMessageAPI extends AbstractModule {
    private long mReceiver = 4105000000131917L;

    private String mMessage = "Hello~ Message Test";

    // message or packet receive event listener
    private HSPMessage.HSPReceiveMessageListener mReceiveMessageListener;

    private HSPMessage.HSPReceivePacketListener mReceivePacketListener;

    private HSPMessage.HSPReceivePushNotificationListener mReceivePushNotificationListener;

    public HSPMessageAPI() {
        super("HSPMessage");
    }

    public void testLoadMessagesFromMessageNo() {
        // A last unique number of read messages, they are assumed to be zero.
        HSPMessage.loadMessagesFromMessageNo(0, 10, new HSPMessage.HSPLoadMessagesCB() {
            @Override
            public void onMessagesLoad(List<HSPMessage> messages, HSPResult result) {
                if (result.isSuccess()) {
                    if (messages.size() == 0) {
                        log("No message list exist.");
                    } else {
                        for (final HSPMessage message : messages) {
                            log("Message number: " + message.getMessageNo());
                            log("Sender member number: " + message.getSenderMemberNo());
                            log("Receiver member number: " + message.getReceiverMemberNo());
                            log("Message Type: " + message.getContentType());
                            log("Message Content: " + message.getContent());
                            log("Message send time: " + message.getSendedDate());
                            log("Sender is Admin: " + message.isSenderAdmin());
                            log("Receiver is Admin: " + message.isReceiverAdmin());
                        }
                    }
                } else {
                    log("Failed to load message list: " + result);
                }
            }
        });
    }

    public void testQueryNewMessageCount() {
        // When asked for the number of unread messages messageCount will be returned via HSPQueryNewMessageCountCB.
        HSPMessage.queryNewMessageCount(new HSPMessage.HSPQueryNewMessageCountCB() {
            @Override
            public void onNewMessageCountReceive(int messageCount, HSPResult result) {
                if (result.isSuccess()) {
                    log("There is " + messageCount + "new messages.");
                } else {
                    log("Failed to load newMessage Count: " + result);
                }
            }
        });
    }

    public void testQueryNewNoticeCount() {
        // When asked for the number of unread notices noticeCount will be returned via HSPQueryNewNoticeCountCB.
        HSPMessage.queryNewNoticeCount(new HSPMessage.HSPQueryNewNoticeCountCB() {
            @Override
            public void onNewNoticeCountReceive(int noticeCount, HSPResult result) {
                if (result.isSuccess()) {
                    log("There is  " + noticeCount + "new notices");
                } else {
                    log("Failed to load new notice count: " + result);
                }
            }
        });
    }

    public void testSendTextMessage() {
        // Send message to user with memberNo(4105000000131917)

        HSPMessage.sendTextMessage(mReceiver, mMessage, new HSPMessage.HSPSendTextMessageCB() {
            @Override
            public void onMessageSend(HSPMessage message, HSPResult result) {
                if (result.isSuccess()) {
                    log("Message transmission has been successful");
                    log("Message number: " + message.getMessageNo());
                    log("Sender member number: " + message.getSenderMemberNo());
                    log("Receiver member number: " + message.getReceiverMemberNo());
                    log("Message Type: " + message.getContentType());
                    log("Message Content: " + message.getContent());
                    log("Message send time: " + message.getSendedDate());
                } else {
                    log("Failed to send text message: " + result);
                }
            }
        });
    }

    public void testSendImageMessage() {
        // Send message to user with memberNo(4105000000131917)
        Bitmap image = ((BitmapDrawable)UiController.getDrawable("test_image")).getBitmap();

        HSPMessage.sendImageMessage(mReceiver, image, new HSPMessage.HSPSendImageMessageCB() {
            @Override
            public void onMessageSend(HSPMessage message, HSPResult result) {
                if (result.isSuccess()) {
                    log("Image Message transmission has been successful");
                    log("Message number: " + message.getMessageNo());
                    log("Sender member number: " + message.getSenderMemberNo());
                    log("Receiver member number: " + message.getReceiverMemberNo());
                    log("Message Type: " + message.getContentType());
                    log("Message Content: " + message.getContent());
                    log("Message send time: " + message.getSendedDate());
                } else {
                    log("Failed to send image message: " + result);
                }
            }
        });
    }

    public void testSendPacket() {
        // Send packet to user with memberNo(4105000000131917)
        byte[] data = mMessage.getBytes();

        HSPMessage.sendPacket(mReceiver, 0, data, new HSPMessage.HSPSendPacketCB() {
            @Override
            public void onPacketSend(HSPResult result) {
                if (result.isSuccess()) {
                    log("Send Packet has been successful.");
                } else {
                    log("Failed to send packet: " + result);
                }
            }
        });
    }

    public void testSendPushNotification() {
        // Send push message to user with memberNo(4105000000131917)

        mReceiver = 799018257320170221L;
        Map<String, String> mapData = new LinkedHashMap<String, String>();
        mapData.put("content", mMessage);
        mapData.put("url", "http://www.hangame.com");
        mapData.put("extraData", "기타데이터");

        HSPMessage.sendPushNotification(mReceiver, "test ~!!", mapData, new HSPMessage.HSPSendPushNotificationCB() {
            @Override
            public void onPushNotificationSend(HSPResult result) {
                if (result.isSuccess()) {
                    log("Send Push message has been successful.");
                } else {
                    log("Failed to Send push message. ( " + result + " )");
                }
            }
        });
    }

    public void testRegisterListener() {
        // An event that is invoked when a message is received
        mReceiveMessageListener = new HSPMessage.HSPReceiveMessageListener() {
            @Override
            public void onMessageReceive(HSPMessage message) {
                // the actions required when handling incoming message
                log("onMessageReceive");
            }
        };

        HSPMessage.addMessageReceiveListener(mReceiveMessageListener);

        // An event that is invoked when a packet is received
        mReceivePacketListener = new HSPMessage.HSPReceivePacketListener() {
            @Override
            public void onPacketReceive(int gameNo, int type, long senderMemberNo, byte[] data) {
                // the actions required when handling incoming packet
                log("onPacketReceive");
            }
        };

        HSPMessage.addPacketReceiveListener(mReceivePacketListener);

        // An event that is invoked when a push message is received
        mReceivePushNotificationListener = new HSPMessage.HSPReceivePushNotificationListener() {
            @Override
            public void onPushNotificationReceive(Map<String, Object> pushData) {
                // the actions required when handling incoming push message
                // log("onPushNotificationReceive");
                //
                // log("extradata :: " + extraData.toString());

                Log.d("nPush", "onPushNotificationReceive :: " + pushData.toString());
                Log.d("nPush", "id " + pushData.get("id") + " :: badge " + pushData.get("badge") + " :: content "
                        + pushData.get("content"));
            }
        };

        HSPMessage.addPushNotificationReceiveListener(mReceivePushNotificationListener);
    }

    public void testEnablePushNotification() {
        boolean result = HSPMessage.setPushNotification(true);
        
        log("Enable PushNotification: result = " + result);
    }

    public void testDisablePushNotification() {
        boolean result = HSPMessage.setPushNotification(false);
        
        log("Disable PushNotification: result = " + result);
    }
    
    public void testIsEnablePushNotification() {
        boolean isEnabled = HSPMessage.isEnablePushNotification();
        
        log("PushNotification is Enabled: " + isEnabled);
    }

    public void testUnregisterListener() {
        HSPMessage.removeMessageReceiveListener(mReceiveMessageListener);
        HSPMessage.removePacketReceiveListener(mReceivePacketListener);
        HSPMessage.removePushNotificationReceiveListener(mReceivePushNotificationListener);
    }
}
