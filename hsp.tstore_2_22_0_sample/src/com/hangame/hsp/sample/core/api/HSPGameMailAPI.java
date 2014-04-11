
package com.hangame.hsp.sample.core.api;

import com.hangame.hsp.HSPGameMail;
import com.hangame.hsp.HSPGameMail.HSPGameMailBoxType;
import com.hangame.hsp.HSPGameMail.HSPLoadGameMailsForPageCB;
import com.hangame.hsp.HSPGameMail.HSPModifyGameMailCB;
import com.hangame.hsp.HSPGameMail.HSPQueryNewGameMailCountCB;
import com.hangame.hsp.HSPGameMail.HSPRemoveGameMailCB;
import com.hangame.hsp.HSPGameMail.HSPSendGameMailCB;
import com.hangame.hsp.HSPResult;
import com.hangame.hsp.sample.AbstractModule;

import java.util.Date;
import java.util.List;

public class HSPGameMailAPI extends AbstractModule {
    private long mReceiver = 4105000000137637L;

    public HSPGameMailAPI() {
        super("HSPGameMail");
    }

    public void testLoadSentGameMailForPage() {
        HSPGameMail.loadGameMails(1, HSPGameMailBoxType.HSP_GAMEMAIL_BOX_OUTGOING, new Date(0), 1, 500, false,
                new HSPLoadGameMailsForPageCB() {

                    @Override
                    public void onGameMailsLoad(List<HSPGameMail> mails, int totalCount, HSPResult result) {
                        if (result.isSuccess()) {
                            if (mails.size() == 0) {
                                log("No Sent gamemail page list exist.");
                            } else {
                                log("Sent gamemail page list!!: " + totalCount);
                                for (final HSPGameMail mail : mails) {
                                    log("MailNo: " + mail.getMailNo());
                                    log("SenderMemberNo: " + mail.getSenderMemberNo());
                                    log("SenderAdmin: " + mail.isSenderAdmin());
                                    log("ReceiverMemberNo: " + mail.getReceiverMemberNo());
                                    log("ReceiverAdmin: " + mail.isReceiverAdmin());
                                    log("ContentType: " + mail.getContentType());
                                    log("Content: " + mail.getContent());
                                    log("SentDate: " + mail.getSentDate());
                                    log("ReceivedDate: " + mail.getReceivedDate());
                                    log("Reserved: " + mail.getReserved());
                                }
                            }
                        } else {
                            log("Failed to load gamemail page list: " + result);
                        }
                    }
                });
    }

    public void testLoadReceivedGameMailForPage() {
        HSPGameMail.loadGameMails(1, HSPGameMailBoxType.HSP_GAMEMAIL_BOX_INCOMING, new Date(0), 1, 500, false,
                new HSPLoadGameMailsForPageCB() {

                    @Override
                    public void onGameMailsLoad(List<HSPGameMail> mails, int totalCount, HSPResult result) {
                        if (result.isSuccess()) {
                            if (mails.size() == 0) {
                                log("No recieved gamemail page list exist.");
                            } else {
                                log("recieved gamemail page list!!: " + totalCount);
                                for (final HSPGameMail mail : mails) {
                                    log("MailNo: " + mail.getMailNo());
                                    log("SenderMemberNo: " + mail.getSenderMemberNo());
                                    log("SenderAdmin: " + mail.isSenderAdmin());
                                    log("ReceiverMemberNo: " + mail.getReceiverMemberNo());
                                    log("ReceiverAdmin: " + mail.isReceiverAdmin());
                                    log("ContentType: " + mail.getContentType());
                                    log("Content: " + mail.getContent());
                                    log("SentDate: " + mail.getSentDate());
                                    log("ReceivedDate: " + mail.getReceivedDate());
                                    log("Reserved: " + mail.getReserved());
                                }
                            }
                        } else {
                            log("Failed to load gamemail page list: " + result);
                        }
                    }
                });
    }

    public void testQueryNewGameMailConunt() {
        HSPGameMail.queryNewGameMailCount(1, new HSPQueryNewGameMailCountCB() {

            @Override
            public void onGameMailCountReceive(int count, HSPResult result) {
                if (result.isSuccess()) {
                    log("new GameMail: " + count);
                } else {
                    log("Failed to query new GameMail: " + result);
                }
            }
        });
    }

    public void testSendGameMail() {
        HSPGameMail.sendGameMail(mReceiver, 1, "test 게임 mail!!!", new HSPSendGameMailCB() {

            @Override
            public void onGameMailSend(HSPGameMail sentMail, HSPResult result) {
                if (result.isSuccess()) {
                    log("Send GameMail has been successful.");
                    log("MailNo: " + sentMail.getMailNo());
                    log("SenderMemberNo: " + sentMail.getSenderMemberNo());
                    log("SenderAdmin: " + sentMail.isSenderAdmin());
                    log("ReceiverMemberNo: " + sentMail.getReceiverMemberNo());
                    log("ReceiverAdmin: " + sentMail.isReceiverAdmin());
                    log("ContentType: " + sentMail.getContentType());
                    log("Content: " + sentMail.getContent());
                    log("SentDate: " + sentMail.getSentDate());
                    log("ReceivedDate: " + sentMail.getReceivedDate());
                    log("Reserved: " + sentMail.getReserved());
                } else {
                    log("Failed to send GameMail: " + result);
                }
            }
        });
    }

    public void testRemoveSentGameMail() {
        HSPGameMail.loadGameMails(1, HSPGameMailBoxType.HSP_GAMEMAIL_BOX_OUTGOING, new Date(0), 1, 500, false,
                new HSPLoadGameMailsForPageCB() {
                    @Override
                    public void onGameMailsLoad(List<HSPGameMail> mails, int totalCount, HSPResult result) {
                        if (result.isSuccess()) {
                            if (mails.size() == 0) {
                                log("No recieved gamemail list exist.");
                            } else {
                                log("Recieved gamemail list!!");
                                for (final HSPGameMail mail : mails) {
                                    mail.removeGameMail(HSPGameMailBoxType.HSP_GAMEMAIL_BOX_OUTGOING,
                                            new HSPRemoveGameMailCB() {

                                                @Override
                                                public void onGameMailRemove(HSPResult result) {
                                                    if (result.isSuccess()) {
                                                        log("Received GameMail remove success!");
                                                    } else {
                                                        log("Failed to Received remove GameMail: " + result);
                                                    }
                                                }
                                            });
                                }
                            }
                        } else {
                            log("Failed to load gamemail list: " + result);
                        }
                    }
                });
    }

    public void testRemoveReceivedGameMail() {
        HSPGameMail.loadGameMails(1, HSPGameMailBoxType.HSP_GAMEMAIL_BOX_INCOMING, new Date(0), 1, 500, false,
                new HSPLoadGameMailsForPageCB() {
                    @Override
                    public void onGameMailsLoad(List<HSPGameMail> mails, int totalCount, HSPResult result) {
                        if (result.isSuccess()) {
                            if (mails.size() == 0) {
                                log("No recieved gamemail list exist.");
                            } else {
                                log("Recieved gamemail list!!");
                                for (final HSPGameMail mail : mails) {
                                    mail.removeGameMail(HSPGameMailBoxType.HSP_GAMEMAIL_BOX_INCOMING,
                                            new HSPRemoveGameMailCB() {

                                                @Override
                                                public void onGameMailRemove(HSPResult result) {
                                                    if (result.isSuccess()) {
                                                        log("Received GameMail remove success!");
                                                    } else {
                                                        log("Failed to Received remove GameMail: " + result);
                                                    }
                                                }
                                            });
                                }
                            }
                        } else {
                            log("Failed to load gamemail list: " + result);
                        }
                    }
                });
    }

    public void testModifyGameMail() {
        HSPGameMail.loadGameMails(1, HSPGameMailBoxType.HSP_GAMEMAIL_BOX_INCOMING, new Date(0), 1, 500, false,
                new HSPLoadGameMailsForPageCB() {
                    @Override
                    public void onGameMailsLoad(List<HSPGameMail> mails, int totalCount, HSPResult result) {
                        if (result.isSuccess()) {
                            if (mails.size() == 0) {
                                log("No recieved gamemail list exist.");
                            } else {
                                log("Recieved gamemail list!!");
                                for (final HSPGameMail mail : mails) {
                                    mail.modifyGameMail(1, "GameMail change !!!", new HSPModifyGameMailCB() {

                                        @Override
                                        public void onGameMailModify(HSPResult result) {
                                            if (result.isSuccess()) {
                                                log("GameMail change success!");
                                            } else {
                                                log("Failed to modify GameMail: " + result);
                                            }
                                        }
                                    });
                                }
                            }
                        } else {
                            log("Failed to load gamemail list: " + result);
                        }
                    }
                });
    }
}
