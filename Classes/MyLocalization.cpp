#include "MyLocalization.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <Foundation/Foundation.h>
#endif

void MyLocal::initLocal()
{	
	CCDictionary* ko = CCDictionary::create();
	ko->setObject(CCString::create("서버에 연결할 수 없습니다."), kMyLocalKey_canNotConnectedServer);
	ko->setObject(CCString::create("화면을 터치 해주세요."), kMyLocalKey_touchPlease);
	ko->setObject(CCString::create("상하좌우 제스처를 이용하여\n많은 영역을 획득해야 하는 게임입니다."), kMyLocalKey_tutorial1);
	ko->setObject(CCString::create("컨트롤 방법에 대해 알려드리겠습니다."), kMyLocalKey_tutorial2);
	ko->setObject(CCString::create("영역을 획득하기 위해서\n오른쪽으로 제스쳐 하세요.\n화면의 모든곳에서 할 수 있습니다."), kMyLocalKey_tutorial3);
	ko->setObject(CCString::create("위로 제스쳐 하세요."), kMyLocalKey_tutorial4);
	ko->setObject(CCString::create("왼쪽으로 제스쳐 하세요."), kMyLocalKey_tutorial5);
	ko->setObject(CCString::create("아래로 제스쳐 하세요."), kMyLocalKey_tutorial6);
	ko->setObject(CCString::create("영역 획득 완료. 잘하셨습니다.\n선을 그리는 도중에 취소를 하려면\n화면을 터치하면 됩니다."), kMyLocalKey_tutorial7);
	ko->setObject(CCString::create("이번엔 획득한 영역 위를 이동하는\n방법을 알려드리겠습니다."), kMyLocalKey_tutorial8);
	ko->setObject(CCString::create("획득한 영역의 외곽선 방향으로\n제스쳐를 하면 해당 방향으로\n외곽선을 따라 이동하게 됩니다."), kMyLocalKey_tutorial9);
	ko->setObject(CCString::create("왼쪽으로 제스쳐를 해보세요."), kMyLocalKey_tutorial10);
	ko->setObject(CCString::create("이동중에 멈추려면 화면을\n살짝 터치하시면 됩니다."), kMyLocalKey_tutorial11);
	ko->setObject(CCString::create("화면을 터치 해보세요."), kMyLocalKey_tutorial12);
	ko->setObject(CCString::create("이제 인터페이스에 대한\n설명을 하겠습니다."), kMyLocalKey_tutorial14);
	ko->setObject(CCString::create("챕터를 결정하는 화면입니다."), kMyLocalKey_tutorial15);
	ko->setObject(CCString::create("표시된 부분이 챕터 입니다.\n하나의 챕터에는 5개의 스테이지가 있습니다.\n스테이지를 모두 클리어 하면\n다음 챕터로 넘어갈 수 있습니다."), kMyLocalKey_tutorial16);
	ko->setObject(CCString::create("아래는 페이스북에\n로그인 하는 부분입니다."), kMyLocalKey_tutorial17);
	ko->setObject(CCString::create("로그인 하시면 친구들의\n점수를 볼 수 있고, 친구와\n아이템을 주고 받을 수 있습니다."), kMyLocalKey_tutorial17_1);
	ko->setObject(CCString::create("챕터를 선택하시면\n지금과 같은 화면을 볼수 있습니다.\n여기서는 스테이지와 속성, 아이템을\n선택할 수 있습니다."), kMyLocalKey_tutorial18);
	ko->setObject(CCString::create("표시된 부분이 스테이지 입니다.\n클리어한 스테이지도 다시 선택해서\n플레이할 수 있습니다."), kMyLocalKey_tutorial19);
	ko->setObject(CCString::create("현재 선택된 스테이지의\n적들 모습과 속성을 보여줍니다."), kMyLocalKey_tutorial20);
	ko->setObject(CCString::create("현재 선택된 속성입니다."), kMyLocalKey_tutorial21);
	ko->setObject(CCString::create("속성을 선택할 수 있으며 골드로\n구매 및 업그레이드할 수 있습니다.\n골드는 게임중에 얻을 수 있습니다."), kMyLocalKey_tutorial22);
	ko->setObject(CCString::create("현재 선택된 아이템 입니다."), kMyLocalKey_tutorial23);
	ko->setObject(CCString::create("아이템을 선택할 수 있으며\n골드로 구매할 수 있습니다."), kMyLocalKey_tutorial24);
	ko->setObject(CCString::create("마지막으로 게임을 시작하는\n버튼 입니다. 게임을 하기 위해서는\n붓이 필요합니다."), kMyLocalKey_tutorial25);
	ko->setObject(CCString::create("튜토리얼을 진행하느라 수고하셨습니다.\n튜토리얼을 종료하고\n챕터를 설정하는 곳으로 갑니다."), kMyLocalKey_tutorial26);
	
	ko->setObject(CCString::create("각 챕터의 마지막 스테이지엔 특별한 이미지가 나옵니다."), kMyLocalKey_loading1);
	ko->setObject(CCString::create("얇게 그리고 싶다면 이동중에 반대방향으로 제스쳐 해보세요."), kMyLocalKey_loading2);
	ko->setObject(CCString::create("모서리부터 그리려면 모서리에 가까워졌을때 해당 방향으로 한번 더 제스쳐 하세요."), kMyLocalKey_loading3);
	ko->setObject(CCString::create("페이스북 로그인을 하시면 친구들의 점수를 볼수 있습니다."), kMyLocalKey_loading4);
	ko->setObject(CCString::create("친구에게 (초대or함께)하기 를 보내면 붓을 얻을 수 있습니다. 한 사람에게 하루에 한번씩 보낼 수 있습니다."), kMyLocalKey_loading5);
	ko->setObject(CCString::create("친구들이 내게 보낸 붓들은 여기서 확인할 수 있습니다."), kMyLocalKey_loading6);
	ko->setObject(CCString::create("공격조건완화 : 영역획득시 더 많은 공격을 합니다."), kMyLocalKey_loading11);
	ko->setObject(CCString::create("시작영역증가 : 더 넓은 영역으로 게임을 시작할 수 있습니다."), kMyLocalKey_loading12);
	ko->setObject(CCString::create("상태이상방어 : 보스의 공격을 방어합니다. 방어되는 공격 : 기절, 수면, 빙결, 혼란"), kMyLocalKey_loading13);
	ko->setObject(CCString::create("속도증가 : 아이템을 획득시 이동속도가 5초 동안 두배로 빨라집니다."), kMyLocalKey_loading14);
	ko->setObject(CCString::create("침묵 : 아이템을 획득시 20초 동안 보스가 공격을 못하도록 합니다."), kMyLocalKey_loading15);
	ko->setObject(CCString::create("속도감소 : 아이템을 획득시 20초 동안 적들의 이동속도를 절반으로 낮춥니다."), kMyLocalKey_loading16);
	ko->setObject(CCString::create("속성을 선택하면 해당 속성에 맞는 공격 스킬이 게임중에 나옵니다."), kMyLocalKey_loading17);
	ko->setObject(CCString::create("속성상관관계"), kMyLocalKey_loading19);
	ko->setObject(CCString::create("지금 페이스북에 로그인하면 BRUSH 5개를 더 드립니다."), kMyLocalKey_loading20);
	
	ko->setObject(CCString::create("공격조건완화"), kMyLocalKey_item1);
	ko->setObject(CCString::create("시작영역증가"), kMyLocalKey_item2);
	ko->setObject(CCString::create("상태이상방어"), kMyLocalKey_item3);
	ko->setObject(CCString::create("속도증가"), kMyLocalKey_item4);
	ko->setObject(CCString::create("침묵"), kMyLocalKey_item5);
	ko->setObject(CCString::create("속도감소"), kMyLocalKey_item6);
	
	ko->setObject(CCString::create("선택된 아이템이 없습니다."), kMyLocalKey_notSelectedItem);
	ko->setObject(CCString::create("이미 선택된 아이템을 구매하셨습니다."), kMyLocalKey_boughtItem);
	ko->setObject(CCString::create("한번의 게임에 사용할 수 있는 아이템 종류는 최대 2가지 입니다."), kMyLocalKey_multiSelect);
	ko->setObject(CCString::create("골드가 부족하여 구매할 수 없습니다."), kMyLocalKey_notEnoughGold);
	ko->setObject(CCString::create("상점으로 가시겠습니까?"), kMyLocalKey_goShop);
	ko->setObject(CCString::create("붓이 부족하여 게임을 시작할 수 없습니다."), kMyLocalKey_notEnoughBrush);
	ko->setObject(CCString::create("루비가 부족하여 구매할 수 없습니다."), kMyLocalKey_notEnoughRuby);
	ko->setObject(CCString::create("게임을 종료하시겠습니까?"), kMyLocalKey_exit);
	ko->setObject(CCString::create("리뷰를 남겨주시면 붓을 가득 채워드립니다."), kMyLocalKey_review);
	ko->setObject(CCString::create("갤러리에 등록 되었습니다. 갤러리로 가시겠습니까?"), kMyLocalKey_regiGallery);
	ko->setObject(CCString::create("어려운 챕터들이 공개되었습니다."), kMyLocalKey_openHard);
	ko->setObject(CCString::create("악의 기운이 가득하여 방어막이 해제됩니다."), kMyLocalKey_shieldCancel);
	ko->setObject(CCString::create("게임 실행 후 보상을 받을 수 있습니다."), kMyLocalKey_exeAppReward);
	ko->setObject(CCString::create("GIFTCODE 는 한시간에 하나만 사용할 수 있습니다."), kMyLocalKey_giftAHour);
	ko->setObject(CCString::create("올바르지 않은 GIFTCODE 입니다."), kMyLocalKey_giftInvalid);
	ko->setObject(CCString::create("이미 사용한 GIFTCODE 입니다."), kMyLocalKey_giftUsed);
	ko->setObject(CCString::create("GIFTCODE는 LitQoo에서 발행합니다."), kMyLocalKey_giftLitqoo);
	ko->setObject(CCString::create("게임을 종료하고 홈으로 이동하시겠습니까?"), kMyLocalKey_goHome);
	ko->setObject(CCString::create("별3개로 클리어 하면 갤러리에 그림이 등록됩니다."), kMyLocalKey_star3clear);
	ko->setObject(CCString::create("아이템을 사용할 수 있습니다."), kMyLocalKey_itemEnable);
	ko->setObject(CCString::create("최신버전으로 업데이트를 해야 플레이가 가능합니다."), kMyLocalKey_needUpdate);
	ko->setObject(CCString::create("최신버전이 출시되었습니다. 업데이트 하시겠습니까?"), kMyLocalKey_newVersion);
	ko->setObject(CCString::create("지금 같이 해요!"), kMyLocalKey_facebookInvite);
	ko->setObject(CCString::create("%s가 BRUSH를 보냈습니다."), kMyLocalKey_facebookWith);
	
	ko->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
	ko->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
	ko->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
	ko->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
	ko->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
	ko->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
	ko->setObject(CCString::create("몬스터를 획득하셨습니다."), kMyLocalKey_getMonster);
	
	ko->setObject(CCString::create("수집한 이미지, 펫, 몬스터의 정보는 컬렉션에서 확인 할 수 있습니다."), kMyLocalKey_loading21);
	ko->setObject(CCString::create("수집한 이미지, 펫, 몬스터의 정보는 컬렉션에서 확인 할 수 있습니다."), kMyLocalKey_loading22);
	ko->setObject(CCString::create("컨트롤 방식은 게임 좌측 화면의 옵션을 통하여 변경 할 수 있습니다."), kMyLocalKey_loading23);
	ko->setObject(CCString::create("펫은 펫샵에서 구매 할 수 있습니다."), kMyLocalKey_loading24);
	ko->setObject(CCString::create("펫의 종류에 따라 (게임 중) 여러 가지 버프 효과를 받을 수 있습니다."), kMyLocalKey_loading25);
	setObject(ko, "ko");
	
	
	CCDictionary* en = CCDictionary::create();
	en->setObject(CCString::create("Cannot connect to the server."), kMyLocalKey_canNotConnectedServer);
	en->setObject(CCString::create("Touch the screen."), kMyLocalKey_touchPlease);
	en->setObject(CCString::create("This is a game in which you\nhave to gain as much area as\npossible using up, down, left,\nand right gestures."), kMyLocalKey_tutorial1);
	en->setObject(CCString::create("I will tell you\nabout the controls."), kMyLocalKey_tutorial2);
	en->setObject(CCString::create("Gesture to the right to\nacquire the area.\nYou can do it anywhere on the screen."), kMyLocalKey_tutorial3);
	en->setObject(CCString::create("Gesture to the top."), kMyLocalKey_tutorial4);
	en->setObject(CCString::create("Gesture to the left."), kMyLocalKey_tutorial5);
	en->setObject(CCString::create("Gesture to the bottom."), kMyLocalKey_tutorial6);
	en->setObject(CCString::create("You’ve finished acquiring\nthe area. Good job. If you\nwant to cancel drawing aline,\nyou can just touch the screen."), kMyLocalKey_tutorial7);
	en->setObject(CCString::create("Now I will tell you how to\nmove over the area you have\nalready acquired."), kMyLocalKey_tutorial8);
	en->setObject(CCString::create("If you gesture along the\nedges of the acquired area,\nyou’ll move in that direction\nalong the edges."), kMyLocalKey_tutorial9);
	en->setObject(CCString::create("Gesture to the left."), kMyLocalKey_tutorial10);
	en->setObject(CCString::create("If you want to stop while\nmoving, just touch the screen\nlightly."), kMyLocalKey_tutorial11);
	en->setObject(CCString::create("Touch the screen."), kMyLocalKey_tutorial12);
	en->setObject(CCString::create("Now I will explain the interface."), kMyLocalKey_tutorial14);
	en->setObject(CCString::create("This is the screen that\ndetermines the chapter."), kMyLocalKey_tutorial15);
	en->setObject(CCString::create("The marked part is the chapter.\nEach chapter has 5 stages.\nOnce you clear all stages,\nyou can move to the next chapter."), kMyLocalKey_tutorial16);
	en->setObject(CCString::create("This is to log in to Facebook."), kMyLocalKey_tutorial17);
	en->setObject(CCString::create("If you log in, you can see\nyour friends’ scores, and\nyou can give and receive\nitems with friends."), kMyLocalKey_tutorial17-1);
	en->setObject(CCString::create("If you select a chapter,\nyou willsee the followingscreen.\nYou can choose the stage,\nproperties, and items."), kMyLocalKey_tutorial18);
	en->setObject(CCString::create("The marked part is the\nstage. You can play stages\nyou have cleared again\nby selecting it."), kMyLocalKey_tutorial19);
	en->setObject(CCString::create("This shows you the selected\nstage’s enemies and properties."), kMyLocalKey_tutorial20);
	en->setObject(CCString::create("This is the selected property."), kMyLocalKey_tutorial21);
	en->setObject(CCString::create("You can select the property,\nand you can buy and upgrade\nwith gold. You can earn gold\nduring the game."), kMyLocalKey_tutorial22);
	en->setObject(CCString::create("This is the item you have\ncurrently selected."), kMyLocalKey_tutorial23);
	en->setObject(CCString::create("You can choose the item,\nand buy things with gold."), kMyLocalKey_tutorial24);
	en->setObject(CCString::create("Finally, this is the game\nstart button. To start the game,\nyou will need a brush."), kMyLocalKey_tutorial25);
	en->setObject(CCString::create("Congratulations on finishing\nthe tutorial. We will now end\nthe tutorial and move on to\nselecting the chapter."), kMyLocalKey_tutorial26);
	
	en->setObject(CCString::create("A special image appears in every chapter."), kMyLocalKey_loading1);
	en->setObject(CCString::create("If you want to draw a thin line, gesture inthe opposite direction as you’re moving."), kMyLocalKey_loading2);
	en->setObject(CCString::create("If you want to draw from the corner, gesture again inthe relevant direction onceyou are close to the corner."), kMyLocalKey_loading3);
	en->setObject(CCString::create("If you log in to Facebook, you can see your friends’ scores."), kMyLocalKey_loading4);
	en->setObject(CCString::create("If you invite your friends, you can earn brushes. You can send one invite per person per day."), kMyLocalKey_loading5);
	en->setObject(CCString::create("You can check the brushes your friends sent you here."), kMyLocalKey_loading6);
	en->setObject(CCString::create("Easy Attack Property: When you acquire an area, you attack more."), kMyLocalKey_loading11);
	en->setObject(CCString::create("Starting Area Increase: You start the game from a wider area."), kMyLocalKey_loading12);
	en->setObject(CCString::create("Block Status Defects: You block attacksby the boss. Attacks that can be blocked: fainting, sleeping, freezing, confusion"), kMyLocalKey_loading13);
	en->setObject(CCString::create("Increase speed: When you get this item, your speed doubles for 5 seconds."), kMyLocalKey_loading14);
	en->setObject(CCString::create("Silence: When you get this item, the boss cannot attack for 20 seconds."), kMyLocalKey_loading15);
	en->setObject(CCString::create("Decreasing speed: When you get this item, the enemies’ movement speed is cut in halffor 20 seconds."), kMyLocalKey_loading16);
	en->setObject(CCString::create("If you select a property, an attack skill that matches the relevant property will come out during the game."), kMyLocalKey_loading17);
	en->setObject(CCString::create("property"), kMyLocalKey_loading19);
	en->setObject(CCString::create("If you log into your Facebook account now, you will receive 5 more BRUSH."), kMyLocalKey_loading20);
	
	en->setObject(CCString::create("Easy Attack Property"), kMyLocalKey_item1);
	en->setObject(CCString::create("Starting Area Increase"), kMyLocalKey_item2);
	en->setObject(CCString::create("Block Status Defects"), kMyLocalKey_item3);
	en->setObject(CCString::create("Increase speed"), kMyLocalKey_item4);
	en->setObject(CCString::create("Silence"), kMyLocalKey_item5);
	en->setObject(CCString::create("Decreasing speed"), kMyLocalKey_item6);
	
	en->setObject(CCString::create("No item has been selected."), kMyLocalKey_notSelectedItem);
	en->setObject(CCString::create("You have already purchased this item."), kMyLocalKey_boughtItem);
	en->setObject(CCString::create("You can select a maximum of 2 items per game."), kMyLocalKey_multiSelect);
	en->setObject(CCString::create("You do not have enough gold for this purchase."), kMyLocalKey_notEnoughGold);
	en->setObject(CCString::create("Do you want to go to the store?"), kMyLocalKey_goShop);
	en->setObject(CCString::create("You cannot start the game because you do nothave any brushes."), kMyLocalKey_notEnoughBrush);
	en->setObject(CCString::create("You do not have enough rubies for this purchase."), kMyLocalKey_notEnoughRuby);
	en->setObject(CCString::create("Do you want to exit the game?"), kMyLocalKey_exit);
	en->setObject(CCString::create("If you leave a review, your brushes will be refilled."), kMyLocalKey_review);
	en->setObject(CCString::create("Recorded to the gallery. Do you want to go to the gallery?"), kMyLocalKey_regiGallery);
	en->setObject(CCString::create("The hard chapters have been revealed."), kMyLocalKey_openHard);
	en->setObject(CCString::create("The shield will disappear because the force of evil is full."), kMyLocalKey_shieldCancel);
	en->setObject(CCString::create("You can receive awards after running the game."), kMyLocalKey_exeAppReward);
	en->setObject(CCString::create("You can only use one GIFTCODE per hour."), kMyLocalKey_giftAHour);
	en->setObject(CCString::create("This is an incorrect GIFTCODE."), kMyLocalKey_giftInvalid);
	en->setObject(CCString::create("This GIFTCODE has already been used."), kMyLocalKey_giftUsed);
	en->setObject(CCString::create("LitQoo issues the GIFTCODE"), kMyLocalKey_giftLitqoo);
	en->setObject(CCString::create("Do you want to go to the home screen and exit the game?"), kMyLocalKey_goHome);
	en->setObject(CCString::create("If you clear with 3 stars, the photo will be registered in the gallery."), kMyLocalKey_star3clear);
	en->setObject(CCString::create("You can use the "), kMyLocalKey_itemEnable);
	en->setObject(CCString::create("You can only play after updating to the newest version."), kMyLocalKey_needUpdate);
	en->setObject(CCString::create("A new version has been released. Do you want to update?"), kMyLocalKey_newVersion);
	en->setObject(CCString::create("Play now!"), kMyLocalKey_facebookInvite);
	en->setObject(CCString::create("%s has sent you a BRUSH."), kMyLocalKey_facebookWith);
	
	en->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
	en->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
	en->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
	en->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
	en->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
	en->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
	en->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
	
	en->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
	en->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
	en->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
	en->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
	en->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
	setObject(en, "en");
	
	
	CCDictionary* ja = CCDictionary::create();
	ja->setObject(CCString::create("サーバーに連結できません。"), kMyLocalKey_canNotConnectedServer);
	ja->setObject(CCString::create("画面をタッチしてください。"), kMyLocalKey_touchPlease);
	ja->setObject(CCString::create("上下左右にジェスチャーを利用し、\n多くのエリアを獲得しなければなら\nないゲームです。"), kMyLocalKey_tutorial1);
	ja->setObject(CCString::create("コントロール方法についてお知らせします。"), kMyLocalKey_tutorial2);
	ja->setObject(CCString::create("エリアを獲得するために右にジェスチャーして\nください。 画面のどこでもできます。"), kMyLocalKey_tutorial3);
	ja->setObject(CCString::create("上にジェスチャーしてください。"), kMyLocalKey_tutorial4);
	ja->setObject(CCString::create("左にジェスチャーしてください。"), kMyLocalKey_tutorial5);
	ja->setObject(CCString::create("下にジェスチャーしてください。"), kMyLocalKey_tutorial6);
	ja->setObject(CCString::create("エリア獲得完了。 よくできました。\n線を描く途中で取消ししたい時は画面を\nタッチしてください。"), kMyLocalKey_tutorial7);
	ja->setObject(CCString::create("今度は獲得したエリアの上を移動する\n方法をお知らせします。"), kMyLocalKey_tutorial8);
	ja->setObject(CCString::create("獲得したエリアの外郭線方向にジェスチ\nャーすると、該当方向に外郭線に従って\n移動します。"), kMyLocalKey_tutorial9);
	ja->setObject(CCString::create("左にジェスチャーしてください。"), kMyLocalKey_tutorial10);
	ja->setObject(CCString::create("移動中に止めたい時は画面に軽く\nタッチしてください。"), kMyLocalKey_tutorial11);
	ja->setObject(CCString::create("画面をタッチしてください。"), kMyLocalKey_tutorial12);
	ja->setObject(CCString::create("ではインターペースについての説明をします。"), kMyLocalKey_tutorial14);
	ja->setObject(CCString::create("チャプターを決定する画面です。"), kMyLocalKey_tutorial15);
	ja->setObject(CCString::create("表示された部分がチャプターです。\n一つのチャプターには5個のステージがあります。\nステージを全てクリアすると次のチャプターに移ります。"), kMyLocalKey_tutorial16);
	ja->setObject(CCString::create("下の説明はフェースブックにログ\nインする部分です。"), kMyLocalKey_tutorial17);
	ja->setObject(CCString::create("ログインすると仲間の点数を見ることができ、\n仲間とアイテムをあげたり貰ったりできます。"), kMyLocalKey_tutorial17-1);
	ja->setObject(CCString::create("チャプターを選択すると今と同じ画面が見れます。\nここではステージと属性、アイテムが選択できます。"), kMyLocalKey_tutorial18);
	ja->setObject(CCString::create("表示された部分がステージ です。クリ\nアしたステージも再び選択してプレーできます。"), kMyLocalKey_tutorial19);
	ja->setObject(CCString::create("現在選択されたステージの敵の姿と\n属性が見えます。"), kMyLocalKey_tutorial20);
	ja->setObject(CCString::create("現在選択された属性です。"), kMyLocalKey_tutorial21);
	ja->setObject(CCString::create("属性を選択することができ、またゴール\nドで購買およびアップグレードできます。\nゴールドはゲーム中に得られます。"), kMyLocalKey_tutorial22);
	ja->setObject(CCString::create("現在選択されたアイテムです。"), kMyLocalKey_tutorial23);
	ja->setObject(CCString::create("アイテムを選択することができ、\nまたゴールドで購買できます。"), kMyLocalKey_tutorial24);
	ja->setObject(CCString::create("最後にゲームを始めるボタンです。\nゲームを始めるためには筆が必要です。"), kMyLocalKey_tutorial25);
	ja->setObject(CCString::create("チュトリアルを進めるのは大変だったで\nしょう。 チュトリアルを終了し、\nチャプターを設定する場所に移動します。"), kMyLocalKey_tutorial26);
	
	ja->setObject(CCString::create("各チャプターの最後のステージには特別なイメージが出ます。"), kMyLocalKey_loading1);
	ja->setObject(CCString::create("薄く描きたいなら移動中に反対方向にジェスチャーしてください。"), kMyLocalKey_loading2);
	ja->setObject(CCString::create("角から描きたいなら角に近くなった時に、該当方向にもう一度ジェスチャーしてください。"), kMyLocalKey_loading3);
	ja->setObject(CCString::create("フェースブックにログインすると仲間達の点数を見ることができます。"), kMyLocalKey_loading4);
	ja->setObject(CCString::create("“仲間に(招待or共に)する”を送ると筆を得られます。一人の人に一日に一度だけ送れます。"), kMyLocalKey_loading5);
	ja->setObject(CCString::create("仲間が私に送った筆はここで確認できます。"), kMyLocalKey_loading6);
	ja->setObject(CCString::create("攻撃条件緩和: エリアを獲得するともっとたくさん攻撃します。"), kMyLocalKey_loading11);
	ja->setObject(CCString::create("始まりエリア増加: もっと広いエリアでゲームを始めることができます。"), kMyLocalKey_loading12);
	ja->setObject(CCString::create("状態異常防御: ボスの攻撃を防御します。防御になる攻撃: 気絶、睡眠、氷結、混乱"), kMyLocalKey_loading13);
	ja->setObject(CCString::create("速度増加: アイテムを獲得すると、移動速度が5秒間二倍に速くなります。"), kMyLocalKey_loading14);
	ja->setObject(CCString::create("沈黙: アイテムを獲得すると20秒間ボスが攻撃できなくなります。"), kMyLocalKey_loading15);
	ja->setObject(CCString::create("速度減少: アイテムを獲得すると20秒間敵の移動速度が半分に下がります。"), kMyLocalKey_loading16);
	ja->setObject(CCString::create("属性を選択すると該当属性に合う攻撃スキルがゲーム中に出てきます。"), kMyLocalKey_loading17);
	ja->setObject(CCString::create("属性"), kMyLocalKey_loading19);
	ja->setObject(CCString::create("今すぐフェースブックにログインされますと BRUSH 5 個を差し上げます。"), kMyLocalKey_loading20);
	
	ja->setObject(CCString::create("攻撃条件緩和"), kMyLocalKey_item1);
	ja->setObject(CCString::create("始まりエリア増加"), kMyLocalKey_item2);
	ja->setObject(CCString::create("状態異常防御"), kMyLocalKey_item3);
	ja->setObject(CCString::create("速度増加"), kMyLocalKey_item4);
	ja->setObject(CCString::create("沈黙"), kMyLocalKey_item5);
	ja->setObject(CCString::create("速度減少"), kMyLocalKey_item6);
	
	ja->setObject(CCString::create("選択されたアイテムがありません。"), kMyLocalKey_notSelectedItem);
	ja->setObject(CCString::create("既に選択されたアイテムを購買しました。"), kMyLocalKey_boughtItem);
	ja->setObject(CCString::create("一度のゲームに使用できるアイテムの種類は最大2種類です。"), kMyLocalKey_multiSelect);
	ja->setObject(CCString::create("ゴールドが足りないので購買できません。"), kMyLocalKey_notEnoughGold);
	ja->setObject(CCString::create("商店に行きますか?"), kMyLocalKey_goShop);
	ja->setObject(CCString::create("筆が足りないのでゲームを始めることができません。"), kMyLocalKey_notEnoughBrush);
	ja->setObject(CCString::create("ルビーが足りないので購買できません。"), kMyLocalKey_notEnoughRuby);
	ja->setObject(CCString::create("ゲームを終了しますか?"), kMyLocalKey_exit);
	ja->setObject(CCString::create("レビューを残してくだされば筆を一杯にしてあげます。"), kMyLocalKey_review);
	ja->setObject(CCString::create("ギャラリーに登録されました。ギャラリーに行きますか?"), kMyLocalKey_regiGallery);
	ja->setObject(CCString::create("難しいチャプターが公開されました。"), kMyLocalKey_openHard);
	ja->setObject(CCString::create("悪い気運が一杯で防御幕が解除されます。"), kMyLocalKey_shieldCancel);
	ja->setObject(CCString::create("ゲーム実行後、保証を貰えます。"), kMyLocalKey_exeAppReward);
	ja->setObject(CCString::create("GIFTCODE は同時に一つだけ使えます。"), kMyLocalKey_giftAHour);
	ja->setObject(CCString::create("正しくない GIFTCODE です。"), kMyLocalKey_giftInvalid);
	ja->setObject(CCString::create("既に使っている GIFTCODE です。"), kMyLocalKey_giftUsed);
	ja->setObject(CCString::create("GIFTCODEは LitQooで発行します。"), kMyLocalKey_giftLitqoo);
	ja->setObject(CCString::create("ゲームを終了しホームに移動しますか?"), kMyLocalKey_goHome);
	ja->setObject(CCString::create("星3個でクリアするとギャラリーに絵が登録されます。"), kMyLocalKey_star3clear);
	ja->setObject(CCString::create("アイテムが使えます。"), kMyLocalKey_itemEnable);
	ja->setObject(CCString::create("最新バージョンにアップデートするとプレーできます。"), kMyLocalKey_needUpdate);
	ja->setObject(CCString::create("最新バージョンがリリースされました。アップデートしますか?"), kMyLocalKey_newVersion);
	ja->setObject(CCString::create("今すぐ一緒にしましょう！"), kMyLocalKey_facebookInvite);
	ja->setObject(CCString::create("%sがBRUSHを送りました。"), kMyLocalKey_facebookWith);
	
	ja->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
	ja->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
	ja->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
	ja->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
	ja->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
	ja->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
	ja->setObject(CCString::create("モンスターを得ました。"), kMyLocalKey_getMonster);
	
	ja->setObject(CCString::create("集まったimageとpetとmonsterの情報はcollectionで確認することができます。"), kMyLocalKey_loading21);
	ja->setObject(CCString::create("集まったimageとpetとmonsterの情報はcollectionで確認することができます。"), kMyLocalKey_loading22);
	ja->setObject(CCString::create("コントロールの方法はゲームの左にあるオプションを利用して変更することができます。"), kMyLocalKey_loading23);
	ja->setObject(CCString::create("ペットはペットショップで買えます。"), kMyLocalKey_loading24);
	ja->setObject(CCString::create("ペットの種類に従って色んなblessingの効果を受けます。"), kMyLocalKey_loading25);
	setObject(ja, "ja");
	
	
	CCDictionary* de = CCDictionary::create();
	de->setObject(CCString::create("Die Verbindung zum Server ist nicht möglich."), kMyLocalKey_canNotConnectedServer);
	de->setObject(CCString::create("Berühre den Bildschirm!"), kMyLocalKey_touchPlease);
	de->setObject(CCString::create("Es ist ein Spiel, dass du\nmit der horizontalen und\nvertikalen Gestik mehrere\nBereiche erwerben sollst."), kMyLocalKey_tutorial1);
	de->setObject(CCString::create("Die Kontrollfunktion läuft\nwie folgt ab."), kMyLocalKey_tutorial2);
	de->setObject(CCString::create("Gestikuliere nach rechts,\num den Bereich zu erwerben!\nDie Gestik funktioniert überall\nauf dem Bildschirm."), kMyLocalKey_tutorial3);
	de->setObject(CCString::create("Gestikuliere nach oben!"), kMyLocalKey_tutorial4);
	de->setObject(CCString::create("Gestikuliere nach links!"), kMyLocalKey_tutorial5);
	de->setObject(CCString::create("Gestikuliere nach unten!"), kMyLocalKey_tutorial6);
	de->setObject(CCString::create("Erwerb des Bereiches abgeschlossen!\nGut gemacht! Berühre den Bildschrim,\num die Funktion abzubrechen\nwährend die Linie gezeichnet wird!"), kMyLocalKey_tutorial7);
	de->setObject(CCString::create("Die Bewegung auf dem erhaltenen\nBereich funktioniert wie folgt."), kMyLocalKey_tutorial8);
	de->setObject(CCString::create("Wenn du deine Finer auf die\nRichtung nach die Umrisslinie\ngestikulierst, bewegt sich die\nLinie entlang der Umrisslinie."), kMyLocalKey_tutorial9);
	de->setObject(CCString::create("Gestikuliere nach links!"), kMyLocalKey_tutorial10);
	de->setObject(CCString::create("Berühre einfach dem Bildschirm,\num zu stoppen während der Bewegung."), kMyLocalKey_tutorial11);
	de->setObject(CCString::create("Berühre dem Bildschirm!"), kMyLocalKey_tutorial12);
	de->setObject(CCString::create("Nun folgt die Erklärung über\ndie Interface-Funktion."), kMyLocalKey_tutorial14);
	de->setObject(CCString::create("Es ist der Bildschrim, wo man\ndas Kapitel auswählen kann."), kMyLocalKey_tutorial15);
	de->setObject(CCString::create("Der markierte Teil ist das Kapitel.\nEin Kapitel besteht aus 5 Stufen.\nWenn alle Stufen gecleart werden,\nkannst du auf das nächste\nKapitel gehen."), kMyLocalKey_tutorial16);
	de->setObject(CCString::create("Unten kannst du im\nFacebook einloggen."), kMyLocalKey_tutorial17);
	de->setObject(CCString::create("Beim Einloggen kannst du\ndie Note deiner Freunde sehen\nund Items austauschen."), kMyLocalKey_tutorial17-1);
	de->setObject(CCString::create("Wenn du das Kapitel auswählst,\nerscheint derselbe Bildschrim wieder.\nHier kannst du Stufen, Merkmalen,\nItems auswählen."), kMyLocalKey_tutorial18);
	de->setObject(CCString::create("Der markierte Teil ist die Stufe.\nDie geclearte Stufe kann\nnochmals gespielt werden,\nwenn du diese Stufe wieder auswählst."), kMyLocalKey_tutorial19);
	de->setObject(CCString::create("Die Figuren und Merkmale der\nFeinden, die von dir derzeit\nausgewählt wurden, werden gezeigt."), kMyLocalKey_tutorial20);
	de->setObject(CCString::create("Es ist das derzeitig\nausgewählte Merkmal."), kMyLocalKey_tutorial21);
	de->setObject(CCString::create("Die Auswahl der Merkmale\nist möglich. Mit Gold kannst\ndu Merkmale kaufen oder\naktualisieren. Während des\nSpiels kannst du Gold erwerben."), kMyLocalKey_tutorial22);
	de->setObject(CCString::create("Es ist das derzeitig\nausgewählte Item."), kMyLocalKey_tutorial23);
	de->setObject(CCString::create("Die Auswahl der Items\nist möglich. Mit dem Gold\nkannst du Items kaufen."), kMyLocalKey_tutorial24);
	de->setObject(CCString::create("Letztes geht es um die\nStarttaste. Der Pinsel ist\nbenötigt, um das Spiel zu beginnen."), kMyLocalKey_tutorial25);
	de->setObject(CCString::create("Danke dir, dass du das\nTutorial erfolgreich nachvollgezogen hast.\nEnde das Tutorial und bewege\ndir auf die Funktion, wo man\ndas Kapitel einstellen kann."), kMyLocalKey_tutorial26);
	
	de->setObject(CCString::create("Ein spezielles Image tritt auf dem letzten Stufen der jeweiligen Kapitels auf."), kMyLocalKey_loading1);
	de->setObject(CCString::create("Wenn du eine schmale Line zeichnen möchtest, gestikuliere in die entgegengesetzte Richtung während der Bewegung."), kMyLocalKey_loading2);
	de->setObject(CCString::create("Wenn du ab dem Rand zeichnen möchtest, gestikuliere in dieselbe Richtug nochmals, wenn die Linie sich an den Rand nährt."), kMyLocalKey_loading3);
	de->setObject(CCString::create("Beim Einloggen im Facebook kannst du die Note der Freunden sehen."), kMyLocalKey_loading4);
	de->setObject(CCString::create("Ein Pinsel ist erhältich, wenn du deine Freunde einlädst oder mitspielst. Einladen oder Mitspielen ist für eine Person einmal pro Tag möglich."), kMyLocalKey_loading5);
	de->setObject(CCString::create("Hier kannst du Pinsels, die deine Freunde gesendet haben, bestätigen."), kMyLocalKey_loading6);
	de->setObject(CCString::create("Erleichterung der Angriffsbedingung: Mehrere Angriffe sind möglich beim Bereich-Erwerb."), kMyLocalKey_loading11);
	de->setObject(CCString::create("Erweiterung des Startbereiches: Das Spiel auf dem größeren Bereich ist möglich."), kMyLocalKey_loading12);
	de->setObject(CCString::create("Abwehr bei der unnormalen Bedingung : Abwehr gegen Boss / Abgewehrte Angriffe : Ohnmacht, Schlafen, Einfrieren, Chaos"), kMyLocalKey_loading13);
	de->setObject(CCString::create("Erhöhung der Geschwindigkeit : Beim Itemerwerb verdoppelt sich die Bewegungsgeschwindigkeit für 5 Sekunden."), kMyLocalKey_loading14);
	de->setObject(CCString::create("Schweigen : Beim Itemerwerb wird der Angriff vom Boss für 20 Sekunden verhindert."), kMyLocalKey_loading15);
	de->setObject(CCString::create("Reduzierung der Geschwindigkeit : Beim Itemerwerb halbiert sich die Bewegungsgeschwindigkeit der Feinde."), kMyLocalKey_loading16);
	de->setObject(CCString::create("Wenn du die Merkmalen auswählst, erscheinen Angriffskills entsprechend der ausgewählten Merkmale während des Spiels."), kMyLocalKey_loading17);
	de->setObject(CCString::create("Merkmalen"), kMyLocalKey_loading19);
	de->setObject(CCString::create("Wenn du dich jetzt bei Facebook einloggen, wirst du 5 zusätzliche BRUSH erhalten."), kMyLocalKey_loading20);
	
	de->setObject(CCString::create("Erleichterung der Angriffsbedingung"), kMyLocalKey_item1);
	de->setObject(CCString::create("Erweiterung des Startbereiches"), kMyLocalKey_item2);
	de->setObject(CCString::create("Abwehr bei der unnormalen Bedingung"), kMyLocalKey_item3);
	de->setObject(CCString::create("Erhöhung der Geschwindigkeit"), kMyLocalKey_item4);
	de->setObject(CCString::create("Schweigen"), kMyLocalKey_item5);
	de->setObject(CCString::create("Reduzierung der Geschwindigkeit"), kMyLocalKey_item6);
	
	de->setObject(CCString::create("Es gibt keine ausgewählte Items."), kMyLocalKey_notSelectedItem);
	de->setObject(CCString::create("Du hast schon die ausgewählte Items gekauft."), kMyLocalKey_boughtItem);
	de->setObject(CCString::create("Die Arten der Items, die in einem Spiel verwendet werden können, sind höchstens 2."), kMyLocalKey_multiSelect);
	de->setObject(CCString::create("Wegen der fehlenden Golds kannst du das Item nicht kaufen."), kMyLocalKey_notEnoughGold);
	de->setObject(CCString::create("Möchtest du in den Einkaufladen gehen?"), kMyLocalKey_goShop);
	de->setObject(CCString::create("Wegen der fehlenden Pinsels kannst du das Spiel nicht starten."), kMyLocalKey_notEnoughBrush);
	de->setObject(CCString::create("Wegen der fehlenden Rubys kannst du das Item nicht kaufen."), kMyLocalKey_notEnoughRuby);
	de->setObject(CCString::create("Möchtest du das Spiel aufhören?"), kMyLocalKey_exit);
	de->setObject(CCString::create("Wenn du hier die Kundenrezension schreibst, wird der Pinsel aufgefüllt."), kMyLocalKey_review);
	de->setObject(CCString::create("Du bist auf der Galerie registriert. Möchtest du in die Galerie gehen?"), kMyLocalKey_regiGallery);
	de->setObject(CCString::create("Schwierige Kapitels wurden veröffentlicht."), kMyLocalKey_openHard);
	de->setObject(CCString::create("Der Abwehrschild wird beseitigt, da die dämonische Atmoshähre herrscht."), kMyLocalKey_shieldCancel);
	de->setObject(CCString::create("Nach der Ausführung des Spiels kannst du Belohnungspunkten bekommen."), kMyLocalKey_exeAppReward);
	de->setObject(CCString::create("GIFTCODE kann innerhalb einer Stunde einmal verwendet werden."), kMyLocalKey_giftAHour);
	de->setObject(CCString::create("GIFTCODE ist falsch."), kMyLocalKey_giftInvalid);
	de->setObject(CCString::create("Dieser GIFTCODE ist schon verwendet."), kMyLocalKey_giftUsed);
	de->setObject(CCString::create("GIFTCODE wird von LitQoo veröffentlicht."), kMyLocalKey_giftLitqoo);
	de->setObject(CCString::create("Möchtest du das Spiel aufhören und auf die Hauptseite gehen?"), kMyLocalKey_goHome);
	de->setObject(CCString::create("Wenn das Spiel mit 3 Sternen gecleart wird, wird das Bild in der Gallerie registriert."), kMyLocalKey_star3clear);
	de->setObject(CCString::create("Item kann verwendet werden."), kMyLocalKey_itemEnable);
	de->setObject(CCString::create("Das Spiel ist erst ermöglicht, wenn die Applikation aktuallisiert wird."), kMyLocalKey_needUpdate);
	de->setObject(CCString::create("Die aktuelle Version wurde veröffentlicht. Möchtest du aktuallisieren?"), kMyLocalKey_newVersion);
	de->setObject(CCString::create("Spiele jetzt das Game zusammen!"), kMyLocalKey_facebookInvite);
	de->setObject(CCString::create("%s hat BRUSH gesendet."), kMyLocalKey_facebookWith);
	
	de->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
	de->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
	de->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
	de->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
	de->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
	de->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
	de->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
	
	de->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
	de->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
	de->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
	de->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
	de->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
	setObject(de, "de");
	
	
	CCDictionary* es = CCDictionary::create();
	es->setObject(CCString::create("No se puede conectar al servidor."), kMyLocalKey_canNotConnectedServer);
	es->setObject(CCString::create("Toca la pantalla."), kMyLocalKey_touchPlease);
	es->setObject(CCString::create("En este juego debes ocupar\nla mayor zona posible\ndeslizándote a los 4 lados."), kMyLocalKey_tutorial1);
	es->setObject(CCString::create("Te enseñaremos la forma de manejo."), kMyLocalKey_tutorial2);
	es->setObject(CCString::create("Desliza hacia la derecha\npara obtener la zona. Puedes\nhacerlo desde todos los\npuntos de la pantalla"), kMyLocalKey_tutorial3);
	es->setObject(CCString::create("Desliza hacia arriba."), kMyLocalKey_tutorial4);
	es->setObject(CCString::create("Desliza hacia la izquierda."), kMyLocalKey_tutorial5);
	es->setObject(CCString::create("Desliza hacia abajo"), kMyLocalKey_tutorial6);
	es->setObject(CCString::create("Obtención de zona complete.\nMuy bien hecho. Para cancelar la\nlínea que estás dibujando, basta\ncon hacer un toque en la pantalla."), kMyLocalKey_tutorial7);
	es->setObject(CCString::create("Ahora te enseñaremos a transladarte\nsobre la zona obtenida."), kMyLocalKey_tutorial8);
	es->setObject(CCString::create("Al deslizarte hacia el contorno\nde la zona obtenida, podrás\nmoverte hacia esa dirección."), kMyLocalKey_tutorial9);
	es->setObject(CCString::create("Deslízate hacia la izquierda."), kMyLocalKey_tutorial10);
	es->setObject(CCString::create("Para detenerte haz un toque\nsuave en la pantalla."), kMyLocalKey_tutorial11);
	es->setObject(CCString::create("Toca la pantalla."), kMyLocalKey_tutorial12);
	es->setObject(CCString::create("Ahora explicaremos la interfaz."), kMyLocalKey_tutorial14);
	es->setObject(CCString::create("Es la pantalla en donde\nse escoje el escenario."), kMyLocalKey_tutorial15);
	es->setObject(CCString::create("La parte marcada es el escenario.\nEn cada capítulo hay 5 escenarios.\nUna vez que hayas despejado\ntodos los escenarios, podrás\npasar al próximo capítulo."), kMyLocalKey_tutorial16);
	es->setObject(CCString::create("La siguiente es la pantalla\nque conecta al Facebook."), kMyLocalKey_tutorial17);
	es->setObject(CCString::create("Al iniciar sesión puedes ver\nla puntuación de tus amigos\ne intercambiar items con ellos."), kMyLocalKey_tutorial17-1);
	es->setObject(CCString::create("Al escoger un capítulo puedes\nver una pantalla como ésta.\nAquí puedes escoger el escenario,\nlos atributos y los items."), kMyLocalKey_tutorial18);
	es->setObject(CCString::create("La parte marcada es el escenario.\nPuedes volver a jugar en uno\nque ya has despejado."), kMyLocalKey_tutorial19);
	es->setObject(CCString::create("Muestra a los enemigos y sus\natributos del escenario escogido."), kMyLocalKey_tutorial20);
	es->setObject(CCString::create("Es el atributo escogido actualmente."), kMyLocalKey_tutorial21);
	es->setObject(CCString::create("Puedes escoger el atributo\no bien adquirirlo o mejorarlo\ncon Gold. Pueder obtener Gold\ndurante el juego."), kMyLocalKey_tutorial22);
	es->setObject(CCString::create("Es el item escogido actualmente."), kMyLocalKey_tutorial23);
	es->setObject(CCString::create("Puedes escoger el item y\nadquirirlo con Gold."), kMyLocalKey_tutorial24);
	es->setObject(CCString::create("Por último, es el botón de\ninicio de juego. Para comenzar\na jugar necesitas de un pincel."), kMyLocalKey_tutorial25);
	es->setObject(CCString::create("Gracias por realizar el\ntutorial. Finaliza el tutorial\ny pasa al ajuste de capítulos."), kMyLocalKey_tutorial26);
	
	es->setObject(CCString::create("En el escenario final de cada capítulo aparece una imagen especial."), kMyLocalKey_loading1);
	es->setObject(CCString::create("Si deseas dibujar una zona angosta, deslízate hacia el lado opuesto durante el movimiento."), kMyLocalKey_loading2);
	es->setObject(CCString::create("Para dibujar desde una esquina, deslízate una vez más cuando casi hayas llegado a ella."), kMyLocalKey_loading3);
	es->setObject(CCString::create("Al iniciar sesión en Facebook puedes ver la puntuación de tus amigos."), kMyLocalKey_loading4);
	es->setObject(CCString::create("Al enviar (invitar o juntos) a un amigo podrás obtener pinceles. Puedes enviar solo una vez por día a cada amigo."), kMyLocalKey_loading5);
	es->setObject(CCString::create("Aquí puedes verificar los pinceles que te han enviado tus amigos."), kMyLocalKey_loading6);
	es->setObject(CCString::create("Mitigación de ataque: realiza más ataques al obtener la zona."), kMyLocalKey_loading11);
	es->setObject(CCString::create("Aumento de la zona de comienzo: Puedes comenzar el juego desde una zona más amplia."), kMyLocalKey_loading12);
	es->setObject(CCString::create("Defensa de ataque: Te defiende el ataque del Boss. Ataque que puede defender: desmayo, sueño, congelado, confusión."), kMyLocalKey_loading13);
	es->setObject(CCString::create("Aumento de velocidad: Al obtener el item, la velocidad de translado aumenta al doble durante 5 segundos."), kMyLocalKey_loading14);
	es->setObject(CCString::create("Silencio: Al obtener el item, el Boss no podrá atacer durante 20 segundos."), kMyLocalKey_loading15);
	es->setObject(CCString::create("Disminución de velocidad: Al obtener el item, disminuye la velocidad de los enemigos a la mitad durante 20 segundos."), kMyLocalKey_loading16);
	es->setObject(CCString::create("Al escoger el atributo, aparecerá durante el juego la habilidad correspondiente."), kMyLocalKey_loading17);
	es->setObject(CCString::create("atributos"), kMyLocalKey_loading19);
	es->setObject(CCString::create("Si se conecta al Facebook ahora, le regalamos 5 BRUSH más."), kMyLocalKey_loading20);
	
	es->setObject(CCString::create("Mitigación de ataque"), kMyLocalKey_item1);
	es->setObject(CCString::create("Aumento de la zona de comienzo"), kMyLocalKey_item2);
	es->setObject(CCString::create("Defensa de ataque"), kMyLocalKey_item3);
	es->setObject(CCString::create("Aumento de velocidad"), kMyLocalKey_item4);
	es->setObject(CCString::create("Silencio"), kMyLocalKey_item5);
	es->setObject(CCString::create("Disminución de velocidad"), kMyLocalKey_item6);
	
	es->setObject(CCString::create("No se ha escogido ningún item."), kMyLocalKey_notSelectedItem);
	es->setObject(CCString::create("Ya has comprador el item escogido."), kMyLocalKey_boughtItem);
	es->setObject(CCString::create("Se pueden utilizar un máximo de 2 items por juego."), kMyLocalKey_multiSelect);
	es->setObject(CCString::create("No tienes Gold suficientes para la compra."), kMyLocalKey_notEnoughGold);
	es->setObject(CCString::create("¿Deseas ir a la tienda?"), kMyLocalKey_goShop);
	es->setObject(CCString::create("No tienes pinceles suficientes para jugar."), kMyLocalKey_notEnoughBrush);
	es->setObject(CCString::create("No tienes Rubíes suficientes para la compra."), kMyLocalKey_notEnoughRuby);
	es->setObject(CCString::create("¿Deseas finalizar el juego?"), kMyLocalKey_exit);
	es->setObject(CCString::create("Si nos dejas una opinión te llenaremos los pinceles."), kMyLocalKey_review);
	es->setObject(CCString::create("Registrado en la galería. ¿Deseas ir a la galería?"), kMyLocalKey_regiGallery);
	es->setObject(CCString::create("Se han abierto los capítulos más difíciles."), kMyLocalKey_openHard);
	es->setObject(CCString::create("Se deshace la capa de defensa por la energía maligna."), kMyLocalKey_shieldCancel);
	es->setObject(CCString::create("Se puede obtener luego de ejecutar el juego."), kMyLocalKey_exeAppReward);
	es->setObject(CCString::create("Se pude utilizar solo un GIFTCODE por hora."), kMyLocalKey_giftAHour);
	es->setObject(CCString::create("GIFTCODE incorrecto."), kMyLocalKey_giftInvalid);
	es->setObject(CCString::create("GIFTCODE ya utilizado."), kMyLocalKey_giftUsed);
	es->setObject(CCString::create("El GIFTCODE se emite desde LitQoo."), kMyLocalKey_giftLitqoo);
	es->setObject(CCString::create("¿Desea terminar el juego y trasladarse a la página principal?"), kMyLocalKey_goHome);
	es->setObject(CCString::create("Al despejar con 3 estrellas, se registrará la imagen en la galleria."), kMyLocalKey_star3clear);
	es->setObject(CCString::create("Puedes utilizar el item"), kMyLocalKey_itemEnable);
	es->setObject(CCString::create("Se necesita actualizar el juego para ejecutarlo."), kMyLocalKey_needUpdate);
	es->setObject(CCString::create("Existe una versión actualizada. ¿Desea actualizar?"), kMyLocalKey_newVersion);
	es->setObject(CCString::create("¡Juguemos ya mismo!"), kMyLocalKey_facebookInvite);
	es->setObject(CCString::create("%s le ha enviado BRUSH."), kMyLocalKey_facebookWith);
	
	es->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
	es->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
	es->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
	es->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
	es->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
	es->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
	es->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
	
	es->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
	es->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
	es->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
	es->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
	es->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
	setObject(es, "es");
	
	
	CCDictionary* fr = CCDictionary::create();
	fr->setObject(CCString::create("La connexion au Serveur n’est pas possible."), kMyLocalKey_canNotConnectedServer);
	fr->setObject(CCString::create("Touchez l’écran."), kMyLocalKey_touchPlease);
	fr->setObject(CCString::create("Ce jeu consiste à obtenir\nbeaucoup de zones en faisant\nglisser le doigt dans tous les sens."), kMyLocalKey_tutorial1);
	fr->setObject(CCString::create("Voilà, c’est l’information\nsur la méthode de contrôle."), kMyLocalKey_tutorial2);
	fr->setObject(CCString::create("Faites glisser le doigt à\ndroite pour obtenir la zone.\nC’est possible à n’importe\nquel endroit de l’écran."), kMyLocalKey_tutorial3);
	fr->setObject(CCString::create("Faites glisser le doigt vers le haut."), kMyLocalKey_tutorial4);
	fr->setObject(CCString::create("Faites glisser le doigt à gauche."), kMyLocalKey_tutorial5);
	fr->setObject(CCString::create("Faites glisser le doigt vers le bas."), kMyLocalKey_tutorial6);
	fr->setObject(CCString::create("Fin d’obtention de la zone.\nBravo! Si vous voulez arrêter\nà dessiner la ligne, il suffit\nde toucher l’écran."), kMyLocalKey_tutorial7);
	fr->setObject(CCString::create("Voilà, c’est l’information\nsur la méthode de déplacement\nsur la zone obtenue."), kMyLocalKey_tutorial8);
	fr->setObject(CCString::create("Si vous faites glisser le\ndoigt vers le sens de la\nligne périphérique de la zone\nobtenue, elle se déplace dans\nla direction correspondante\nen suivant la ligne périphérique."), kMyLocalKey_tutorial9);
	fr->setObject(CCString::create("Faites glisser le doigt à gauche."), kMyLocalKey_tutorial10);
	fr->setObject(CCString::create("Si vous voulez vous arrêter\nen cours du mouvement, touchez\nlégèrement l’écran."), kMyLocalKey_tutorial11);
	fr->setObject(CCString::create("Touchez l’écran."), kMyLocalKey_tutorial12);
	fr->setObject(CCString::create("Voilà, c’est l’explication\nsur l’interface."), kMyLocalKey_tutorial14);
	fr->setObject(CCString::create("C’est un écran à decider le chapître."), kMyLocalKey_tutorial15);
	fr->setObject(CCString::create("Le point marqué , c’est le chapître.\nUn chapître comprend les 5 scènes.\nSi toutes les scènes sont terminées,\nil possible de passer pour\nle chapître suivant."), kMyLocalKey_tutorial16);
	fr->setObject(CCString::create("En bas, il y a une partie\npour l’ouverture de\nsession sur Facebook."), kMyLocalKey_tutorial17);
	fr->setObject(CCString::create("Suite à l’ouverture de\nsession, il est possible de\nregarder les notes des amis\net d’échanger les items avec eux."), kMyLocalKey_tutorial17-1);
	fr->setObject(CCString::create("Si vous sélectionnez le\nchapître, il est possible de\nregarder le même écran comme\nmaintenant. Ici, il est possible\nde sélectionner la scène, les\ncaractéristiques et les items."), kMyLocalKey_tutorial18);
	fr->setObject(CCString::create("Le point marquée, c’est la\nscène. Il est possible de\nsélectionner de nouveau et\nde jouer à la scène déja passée."), kMyLocalKey_tutorial19);
	fr->setObject(CCString::create("Cela montre les images et\nles caractéristiques des\nennemis de la scène\nsélectionnée pour le moment."), kMyLocalKey_tutorial20);
	fr->setObject(CCString::create("Ce sont les caractéristiques\nsélectionnées pour le moment."), kMyLocalKey_tutorial21);
	fr->setObject(CCString::create("Il est possible de sélectionner\nles caractéristiques et de\nles acheter et de mettre à\nniveau avec l’or. Il est\npossible d’obtenir l’or\nen cours du jeu."), kMyLocalKey_tutorial22);
	fr->setObject(CCString::create("Ce sont les items sélectionnés\npour le moment."), kMyLocalKey_tutorial23);
	fr->setObject(CCString::create("Il est possible de sélectionner\nles items et de les acheter avec l’or."), kMyLocalKey_tutorial24);
	fr->setObject(CCString::create("C’est le bouton pour démarrer\nle jeu pour la dernière fois.\nLe pinceau est nécessaire\nà commencer le jeu."), kMyLocalKey_tutorial25);
	fr->setObject(CCString::create("Vous avez bien réalisé le\nTutoriel. Le Tutoriel est\nterminé et déplacez sur le lieu\nà sélectionner le chapître."), kMyLocalKey_tutorial26);
	
	fr->setObject(CCString::create("Un image spécial sera apparu sur la dernière scène de chaque chapître."), kMyLocalKey_loading1);
	fr->setObject(CCString::create("Si vous voulez dessiner de manière plus fine, faites glisser le doigt vers le sens opposé."), kMyLocalKey_loading2);
	fr->setObject(CCString::create("Pour dessiner à partir d’un angle, faites glisser encore le doigt vers le sens correspondant auprès de l’angle."), kMyLocalKey_loading3);
	fr->setObject(CCString::create("Suite à l’ouverture de session sur Facebook, il est possible de regarder les notes des amis."), kMyLocalKey_loading4);
	fr->setObject(CCString::create("Si vous envoyez le message (Invitation or Ensemble), vous pouvez obtenir les pinceaux. Il est possible d’envoyer le message à une personne une fois par jour."), kMyLocalKey_loading5);
	fr->setObject(CCString::create("Il est possible de vérifier les pinceaux envoyés par les amis."), kMyLocalKey_loading6);
	fr->setObject(CCString::create("Adoucissement des conditions d’attaque; Il est possible de faire plus beaucoup d’attaques lors de l’obtention de la zone."), kMyLocalKey_loading11);
	fr->setObject(CCString::create("Elargissement de la zone de démarrage; Il est possible de démarrer le jeu dans la zone plus large."), kMyLocalKey_loading12);
	fr->setObject(CCString::create("Défense de la situation anormale; cela permet de défendre contre l’attaque de boss. Attaques à défendre; évanouissement, sommeil, congélation, désordre."), kMyLocalKey_loading13);
	fr->setObject(CCString::create("Accélération; La vitesse sera accélérée deux fois plus rapide pendant 5 secondes après l’obtention d’un item."), kMyLocalKey_loading14);
	fr->setObject(CCString::create("Silence; Le Boss ne peut pas faire une attaque pendant 20 secondes après l’obtention d’un item."), kMyLocalKey_loading15);
	fr->setObject(CCString::create("Décélération;  La vitesse de déplacement des ennemis sera réduite à moitié pendant 20 secondes après l’obtention d’un item."), kMyLocalKey_loading16);
	fr->setObject(CCString::create("Si vous sélectionnez une caractéristique, une technique d’attaque correspondante sera apparue en cours du jeu."), kMyLocalKey_loading17);
	fr->setObject(CCString::create("caractéristiques"), kMyLocalKey_loading19);
	fr->setObject(CCString::create("Si vous ouvrez maintenant la session sur Facebook , vous aurez encore 5 BRUSH."), kMyLocalKey_loading20);
	
	fr->setObject(CCString::create("Adoucissement des conditions d’attaque"), kMyLocalKey_item1);
	fr->setObject(CCString::create("Elargissement de la zone de démarrage"), kMyLocalKey_item2);
	fr->setObject(CCString::create("Défense de la situation anormale"), kMyLocalKey_item3);
	fr->setObject(CCString::create("Accélération"), kMyLocalKey_item4);
	fr->setObject(CCString::create("Silence"), kMyLocalKey_item5);
	fr->setObject(CCString::create("Décélération"), kMyLocalKey_item6);
	
	fr->setObject(CCString::create("Vous n’avez pas sélectionné les items."), kMyLocalKey_notSelectedItem);
	fr->setObject(CCString::create("Vous avez déjà acheté les items sélectionnés."), kMyLocalKey_boughtItem);
	fr->setObject(CCString::create("Il est possible d’utiliser deux types d’item au maximum pour un seul jeu."), kMyLocalKey_multiSelect);
	fr->setObject(CCString::create("L’achat n’est pas possible d’un manque d’or"), kMyLocalKey_notEnoughGold);
	fr->setObject(CCString::create("Voulez-vous entrer dans le magasin?"), kMyLocalKey_goShop);
	fr->setObject(CCString::create("S’il n’y a pas assez de pinceau, il ne sera pas possible de démarrer le jeu."), kMyLocalKey_notEnoughBrush);
	fr->setObject(CCString::create("L’achat n’est pas possible d’un manque de rubis"), kMyLocalKey_notEnoughRuby);
	fr->setObject(CCString::create("Voulez-vous terminer le jeu?"), kMyLocalKey_exit);
	fr->setObject(CCString::create("Si vous laissez quelques mots sur la revue, vous aurez plein de pinceaux."), kMyLocalKey_review);
	fr->setObject(CCString::create("Vous êtes inscrit à la galerie. Voulez-vous visiter la galerie?"), kMyLocalKey_regiGallery);
	fr->setObject(CCString::create("Les chapîtres difficiles sont sortis."), kMyLocalKey_openHard);
	fr->setObject(CCString::create("La clôture de protection est dégagée par beaucoup de tendance malheureuse."), kMyLocalKey_shieldCancel);
	fr->setObject(CCString::create("La récompense sera donnée après le démarrage du jeu."), kMyLocalKey_exeAppReward);
	fr->setObject(CCString::create("Possible d’utiliser un seul GIFTCODE pendant une heure."), kMyLocalKey_giftAHour);
	fr->setObject(CCString::create("Ce GIFTCODE n’est pas correct."), kMyLocalKey_giftInvalid);
	fr->setObject(CCString::create("Ce GIFTCODE a été déjà utilisé."), kMyLocalKey_giftUsed);
	fr->setObject(CCString::create("GIFTCODE est diffusé par LitQoo"), kMyLocalKey_giftLitqoo);
	fr->setObject(CCString::create("Voulez- vous terminer le jeu et retourner à la page d’accueil?"), kMyLocalKey_goHome);
	fr->setObject(CCString::create("Si il est possible de passer au niveau supérieur, la peinture sera inscrite dans la galerie."), kMyLocalKey_star3clear);
	fr->setObject(CCString::create("Il est possible d’utiliser l’item"), kMyLocalKey_itemEnable);
	fr->setObject(CCString::create("Possible de faire du jeu suite au téléchargement de la nouvelle version."), kMyLocalKey_needUpdate);
	fr->setObject(CCString::create("Une nouvelle version est sortie. Voulez- vous la télécharger?"), kMyLocalKey_newVersion);
	fr->setObject(CCString::create("Faisons- le ensemble!"), kMyLocalKey_facebookInvite);
	fr->setObject(CCString::create("%s a envoyé BRUSH."), kMyLocalKey_facebookWith);
	
	fr->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
	fr->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
	fr->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
	fr->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
	fr->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
	fr->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
	fr->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
	
	fr->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
	fr->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
	fr->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
	fr->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
	fr->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
	setObject(fr, "fr");
	
	
	CCDictionary* zh = CCDictionary::create();
	zh->setObject(CCString::create("无法连接服务器。"), kMyLocalKey_canNotConnectedServer);
	zh->setObject(CCString::create("请触摸画面。"), kMyLocalKey_touchPlease);
	zh->setObject(CCString::create("利用上下左右滑动要多获得领域的游戏。"), kMyLocalKey_tutorial1);
	zh->setObject(CCString::create("对控制方法的说明。"), kMyLocalKey_tutorial2);
	zh->setObject(CCString::create("为获得领域请向右滑动。\n在画面所有的角落都可以做到。"), kMyLocalKey_tutorial3);
	zh->setObject(CCString::create("向上滑动。"), kMyLocalKey_tutorial4);
	zh->setObject(CCString::create("向左滑动。"), kMyLocalKey_tutorial5);
	zh->setObject(CCString::create("向下滑动。"), kMyLocalKey_tutorial6);
	zh->setObject(CCString::create("领域获得结束。 做的很好。\n在画线的过程中想取消就请触摸画面。"), kMyLocalKey_tutorial7);
	zh->setObject(CCString::create("这一次，说明对获得的领域向上移动的方法。"), kMyLocalKey_tutorial8);
	zh->setObject(CCString::create("以获得领域的外轮廓方向滑动，\n在相应的方向会根据外轮廓移动。"), kMyLocalKey_tutorial9);
	zh->setObject(CCString::create("请向左滑动。"), kMyLocalKey_tutorial10);
	zh->setObject(CCString::create("移动中想停止就轻轻地触摸。"), kMyLocalKey_tutorial11);
	zh->setObject(CCString::create("请触摸画面。"), kMyLocalKey_tutorial12);
	zh->setObject(CCString::create("现在对接口说明一下。"), kMyLocalKey_tutorial14);
	zh->setObject(CCString::create("决定篇章的画面。"), kMyLocalKey_tutorial15);
	zh->setObject(CCString::create("表示的部分就是篇章。每个篇章里有5个阶段。\n阶段都过了关就会进入下个篇章。"), kMyLocalKey_tutorial16);
	zh->setObject(CCString::create("下面是对脸谱注册的部分。"), kMyLocalKey_tutorial17);
	zh->setObject(CCString::create("注册会看到朋友们的分数，\n可以和朋友传接商品。"), kMyLocalKey_tutorial17-1);
	zh->setObject(CCString::create("选择篇章就会看到现在的画面。\n在这里可以选择阶段和属性，商品。"), kMyLocalKey_tutorial18);
	zh->setObject(CCString::create("表示的部分就是阶段。\n已过的阶段也可以重选玩。"), kMyLocalKey_tutorial19);
	zh->setObject(CCString::create("显现当前被选阶段的对抗者模样和属性。"), kMyLocalKey_tutorial20);
	zh->setObject(CCString::create("当前被选的属性。"), kMyLocalKey_tutorial21);
	zh->setObject(CCString::create("可以选择属性，以黄金购买及升级。\n黄金可在游戏中获得。"), kMyLocalKey_tutorial22);
	zh->setObject(CCString::create("是当前被选的商品。"), kMyLocalKey_tutorial23);
	zh->setObject(CCString::create("可以选择商品也可用黄金购买。"), kMyLocalKey_tutorial24);
	zh->setObject(CCString::create("最后是游戏开始按钮。\n为开始游戏需要有画笔。"), kMyLocalKey_tutorial25);
	zh->setObject(CCString::create("为进行教程辛苦啦。\n结束教程向设定篇章方向移动。"), kMyLocalKey_tutorial26);
	
	zh->setObject(CCString::create("个篇章的最后阶段出现特别的形象。"), kMyLocalKey_loading1);
	zh->setObject(CCString::create("想画的薄一些就请在移动中相反方向滑动。"), kMyLocalKey_loading2);
	zh->setObject(CCString::create("想从角落开始画起，那就要在靠近角落时在相应的方向再次滑动。"), kMyLocalKey_loading3);
	zh->setObject(CCString::create("注册脸谱可看到朋友们的分数。"), kMyLocalKey_loading4);
	zh->setObject(CCString::create("向朋友发送（邀请 or 一起）可获得画笔。可以向每个人一天发送一次。"), kMyLocalKey_loading5);
	zh->setObject(CCString::create("朋友发送过来的画笔可以在这里确认。"), kMyLocalKey_loading6);
	zh->setObject(CCString::create("攻击条件放宽：获得领域时攻击更多。"), kMyLocalKey_loading11);
	zh->setObject(CCString::create("开始领域增加：在更宽的领域开始游戏。"), kMyLocalKey_loading12);
	zh->setObject(CCString::create("状态异常防御：防御头领的攻击。防御起来的攻击：气绝，睡眠，冷冻，发昏"), kMyLocalKey_loading13);
	zh->setObject(CCString::create("速度增加：获得商品时移动速度5秒钟增加两倍快速。"), kMyLocalKey_loading14);
	zh->setObject(CCString::create("沉默：获得商品时20秒钟不让头领攻击。"), kMyLocalKey_loading15);
	zh->setObject(CCString::create("减速：获得商品时20秒钟将对抗者的移动速度减半。"), kMyLocalKey_loading16);
	zh->setObject(CCString::create("选择属性，游戏中就会显现符合该属性的攻击技术。"), kMyLocalKey_loading17);
	zh->setObject(CCString::create("属性"), kMyLocalKey_loading19);
	zh->setObject(CCString::create("现在登录脸谱网的话多送5个BRUSH"), kMyLocalKey_loading20);
	
	zh->setObject(CCString::create("攻击条件放宽"), kMyLocalKey_item1);
	zh->setObject(CCString::create("开始领域增加"), kMyLocalKey_item2);
	zh->setObject(CCString::create("状态异常防御"), kMyLocalKey_item3);
	zh->setObject(CCString::create("速度增加"), kMyLocalKey_item4);
	zh->setObject(CCString::create("沉默"), kMyLocalKey_item5);
	zh->setObject(CCString::create("减速"), kMyLocalKey_item6);
	
	zh->setObject(CCString::create("没有被选商品。"), kMyLocalKey_notSelectedItem);
	zh->setObject(CCString::create("购买了已被选择的商品。"), kMyLocalKey_boughtItem);
	zh->setObject(CCString::create("一次游戏中可使用的商品种类为最多2种。"), kMyLocalKey_multiSelect);
	zh->setObject(CCString::create("缺少黄金无法购买。"), kMyLocalKey_notEnoughGold);
	zh->setObject(CCString::create("去商店吗？"), kMyLocalKey_goShop);
	zh->setObject(CCString::create("缺少画笔无法开始游戏。"), kMyLocalKey_notEnoughBrush);
	zh->setObject(CCString::create("缺少红宝石无法购买。"), kMyLocalKey_notEnoughRuby);
	zh->setObject(CCString::create("结束游戏吗？"), kMyLocalKey_exit);
	zh->setObject(CCString::create("留下评论会填满画笔。"), kMyLocalKey_review);
	zh->setObject(CCString::create("登录画廊。去画廊吗？"), kMyLocalKey_regiGallery);
	zh->setObject(CCString::create("公开了高难篇章。"), kMyLocalKey_openHard);
	zh->setObject(CCString::create("充满了恶意防御盾解除。"), kMyLocalKey_shieldCancel);
	zh->setObject(CCString::create("执行游戏后可得到赔偿金"), kMyLocalKey_exeAppReward);
	zh->setObject(CCString::create("GIFTCODE 1小时只能使用一次"), kMyLocalKey_giftAHour);
	zh->setObject(CCString::create("是不正确的 GIFTCODE"), kMyLocalKey_giftInvalid);
	zh->setObject(CCString::create("是以使用过的 GIFTCODE"), kMyLocalKey_giftUsed);
	zh->setObject(CCString::create("GIFTCODE发生在LitQoo。"), kMyLocalKey_giftLitqoo);
	zh->setObject(CCString::create("结束游戏转移到首页吗？"), kMyLocalKey_goHome);
	zh->setObject(CCString::create("用3个星清除的话画廊里会登录图片"), kMyLocalKey_star3clear);
	zh->setObject(CCString::create("可以使用商品"), kMyLocalKey_itemEnable);
	zh->setObject(CCString::create("升级到最新版本才可能玩"), kMyLocalKey_needUpdate);
	zh->setObject(CCString::create("推出最新版本。升级吗？"), kMyLocalKey_newVersion);
	zh->setObject(CCString::create("现在一起来吧！"), kMyLocalKey_facebookInvite);
	zh->setObject(CCString::create("%s 送来 BRUSH"), kMyLocalKey_facebookWith);
	
	zh->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
	zh->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
	zh->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
	zh->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
	zh->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
	zh->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
	zh->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
	
	zh->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
	zh->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
	zh->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
	zh->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
	zh->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
	setObject(zh, "zh");
	
	
	CCDictionary* it = CCDictionary::create();
	it->setObject(CCString::create("Impossibile connettersi al server."), kMyLocalKey_canNotConnectedServer);
	it->setObject(CCString::create("Tocca il display."), kMyLocalKey_touchPlease);
	it->setObject(CCString::create("E’ un gioco con cui dovrai\nconquistare più territori con\nmovimenti su e  giù/destra\ne sinistra."), kMyLocalKey_tutorial1);
	it->setObject(CCString::create("Ora ti spieghiamo il controllo."), kMyLocalKey_tutorial2);
	it->setObject(CCString::create("Per conquistare il territorio\nmuoviti verso destra. Puoi\nfarlo in tutte le parti del display."), kMyLocalKey_tutorial3);
	it->setObject(CCString::create("Muoviti in su."), kMyLocalKey_tutorial4);
	it->setObject(CCString::create("Muoviti verso sinistra."), kMyLocalKey_tutorial5);
	it->setObject(CCString::create("Muoviti in giù."), kMyLocalKey_tutorial6);
	it->setObject(CCString::create("Completatala conquista del\nterritorio. Hai fatto un buon\nlavoro. Per cancellare\nl’operazione mentre tracci la\nlinea, basta toccare il display."), kMyLocalKey_tutorial7);
	it->setObject(CCString::create("Ora ti spieghiamo come spostarsi\nsopra il territorio conquistato."), kMyLocalKey_tutorial8);
	it->setObject(CCString::create("Muovendoti verso il contorno\ndel territorio conquistato, puoi\nspostarti lungo il contorno\nin quella direzione."), kMyLocalKey_tutorial9);
	it->setObject(CCString::create("Prova a muoverti verso sinistra."), kMyLocalKey_tutorial10);
	it->setObject(CCString::create("Per fermarti durante lo\nspostamento, basta toccare\nleggermente il display."), kMyLocalKey_tutorial11);
	it->setObject(CCString::create("Prova a toccare il display."), kMyLocalKey_tutorial12);
	it->setObject(CCString::create("Ora ti spieghiamo l’interfaccia."), kMyLocalKey_tutorial14);
	it->setObject(CCString::create("E’ il display che determina\nil capitolo."), kMyLocalKey_tutorial15);
	it->setObject(CCString::create("La parte segnata è un capitolo.\nIn un capitolo ci sono 5 fasi.\nAzzerando tutte le fasi, puoi\npassare al prossimo capitolo."), kMyLocalKey_tutorial16);
	it->setObject(CCString::create("Sotto trovi l’Area Accesso a Facebook."), kMyLocalKey_tutorial17);
	it->setObject(CCString::create("Accedendo a Facebook, puoi\nvedere i punteggi degli amici\ne scambiare oggetti con loro."), kMyLocalKey_tutorial17-1);
	it->setObject(CCString::create("Selezionando un capitolo,\npuoi vedere il display come\nquesto. Qui puoi selezionare\nla fase, la caratteristica\ne l’oggetto."), kMyLocalKey_tutorial18);
	it->setObject(CCString::create("La parte segnata è la fase.\nAnche la fase azzerata può\nessere giocata di\nnuovo selezionandola."), kMyLocalKey_tutorial19);
	it->setObject(CCString::create("Vengono visualizzati gli\naspetti e le caratteristiche\ndella fase attualmente selezionata."), kMyLocalKey_tutorial20);
	it->setObject(CCString::create("Sono le caratteristiche selezionate."), kMyLocalKey_tutorial21);
	it->setObject(CCString::create("Puoi selezionare, acquistare\nle caratteristiche con Oro e\nmigliorarle. Puoi ottenere\nOro durante il gioco."), kMyLocalKey_tutorial22);
	it->setObject(CCString::create("E’ l’oggetto attualmente selezionato."), kMyLocalKey_tutorial23);
	it->setObject(CCString::create("Puoi selezionare e\nacquistare l’oggetto con Oro."), kMyLocalKey_tutorial24);
	it->setObject(CCString::create("E’ il tasto per iniziare\nil gioco per l’ultima volta.\nPer iniziarlo hai bisogno\ndi un pennello."), kMyLocalKey_tutorial25);
	it->setObject(CCString::create("Grazie per aver appreso il\ntutorial. Ora terminiamo il\ntutorial e spostiamoci nell’Area\nImpostazione Capitolo."), kMyLocalKey_tutorial26);
	
	it->setObject(CCString::create("Nella fase finale di ogni capitolo si visualizza un’immagine speciale."), kMyLocalKey_loading1);
	it->setObject(CCString::create("Se vuoi tracciare la linea sottile, muoviti nella direzione opposta durante lo spostamento."), kMyLocalKey_loading2);
	it->setObject(CCString::create("Se vuoi tracciare a cominciare da unangolo, quando ti avvicini all’angolo muoviti un’altra volta in quella direzione."), kMyLocalKey_loading3);
	it->setObject(CCString::create("Accedendo a Facebook, puoi vedere i punteggi degli amici."), kMyLocalKey_loading4);
	it->setObject(CCString::create("Se inviti o condividi il gioco con amici puoi ricevere un pennello. Puoi inviare la richiesta a un amico una volta al giorno."), kMyLocalKey_loading5);
	it->setObject(CCString::create("Puoi visualizzare qui i pennelli ricevuti dagli amici."), kMyLocalKey_loading6);
	it->setObject(CCString::create("Facilitazione della condizione d’attacco: Puoi attaccare di più quando conquisti il territorio."), kMyLocalKey_loading11);
	it->setObject(CCString::create("Aumento dell’Area Iniziale: Puoi iniziare il gioco in un’area più ampia."), kMyLocalKey_loading12);
	it->setObject(CCString::create("Difesa allo stato anormale: Puoi difenderti dagli attacchi del capo. Attacchi da difendere: svenimento, sonno, congelamento, confusione"), kMyLocalKey_loading13);
	it->setObject(CCString::create("Aumento della velocità: Quando ottieni l’oggetto, la velocità dello spostamento si raddoppia per 5 secondi."), kMyLocalKey_loading14);
	it->setObject(CCString::create("Silenzio: Quando ottieni l’oggetto, il capo non ti può attaccare per 20 secondi."), kMyLocalKey_loading15);
	it->setObject(CCString::create("Riduzione della velocità: Quando ottieni l’oggetto, la velocità dello spostamento dei nemici si dimezza per 20 secondi."), kMyLocalKey_loading16);
	it->setObject(CCString::create("Quando selezioni la caratteristica, il trucco d’attacco si visualizza durante il gioco."), kMyLocalKey_loading17);
	it->setObject(CCString::create("caratteristica"), kMyLocalKey_loading19);
	it->setObject(CCString::create("Se accedi adesso a Facebook, ti regaliamo altri 5 BRUSH."), kMyLocalKey_loading20);
	
	
	it->setObject(CCString::create("Facilitazione della condizione d’attacco"), kMyLocalKey_item1);
	it->setObject(CCString::create("Aumento dell’Area Iniziale"), kMyLocalKey_item2);
	it->setObject(CCString::create("Difesa allo stato anormale"), kMyLocalKey_item3);
	it->setObject(CCString::create("Aumento della velocità"), kMyLocalKey_item4);
	it->setObject(CCString::create("Silenzio"), kMyLocalKey_item5);
	it->setObject(CCString::create("Riduzione della velocità"), kMyLocalKey_item6);
	
	it->setObject(CCString::create("Nessun oggetto selezionato."), kMyLocalKey_notSelectedItem);
	it->setObject(CCString::create("Hai già acquistato l’oggetto selezionato."), kMyLocalKey_boughtItem);
	it->setObject(CCString::create("Puoi usare max. 2 tipi di oggetti per un gioco."), kMyLocalKey_multiSelect);
	it->setObject(CCString::create("Oro insufficiente per fare un acquisto."), kMyLocalKey_notEnoughGold);
	it->setObject(CCString::create("Vuoiandare al negozio?"), kMyLocalKey_goShop);
	it->setObject(CCString::create("Pennello insufficiente per iniziare il gioco."), kMyLocalKey_notEnoughBrush);
	it->setObject(CCString::create("Rubino insufficiente per fare un acquisto."), kMyLocalKey_notEnoughRuby);
	it->setObject(CCString::create("Vuoiterminareilgioco?"), kMyLocalKey_exit);
	it->setObject(CCString::create("Se lasci i commenti, ti riempiamo di pennelli."), kMyLocalKey_review);
	it->setObject(CCString::create("E’ stato registrato alla galleria. Vuoiandarealla galleria?"), kMyLocalKey_regiGallery);
	it->setObject(CCString::create("Si sono rivelati i capitoli difficili."), kMyLocalKey_openHard);
	it->setObject(CCString::create("La forza del male disattiva la protezione."), kMyLocalKey_shieldCancel);
	it->setObject(CCString::create("Puoi prendere la compensazione dopo aver eseguito il gioco. "), kMyLocalKey_exeAppReward);
	it->setObject(CCString::create("GIFTCODE può essere utilizzato entro un’ora."), kMyLocalKey_giftAHour);
	it->setObject(CCString::create("GIFTCODE non corretto."), kMyLocalKey_giftInvalid);
	it->setObject(CCString::create("GIFTCODE già utilizzato."), kMyLocalKey_giftUsed);
	it->setObject(CCString::create("GIFTCODE viene rilasciato a LitQoo."), kMyLocalKey_giftLitqoo);
	it->setObject(CCString::create("Vuoi terminare il gioco e spostarti sulla pagina principale?"), kMyLocalKey_goHome);
	it->setObject(CCString::create("Azzerando con tre stelle, l’immagine verrà registrata nella galleria."), kMyLocalKey_star3clear);
	it->setObject(CCString::create("Puoi utilizzare l’oggetto"), kMyLocalKey_itemEnable);
	it->setObject(CCString::create("E’ possibile giocare solo con la versione aggiornata."), kMyLocalKey_needUpdate);
	it->setObject(CCString::create("E’ uscita l’ultima versione. Vuoi aggiornarla?"), kMyLocalKey_newVersion);
	it->setObject(CCString::create("Divertiamoci insieme adesso!"), kMyLocalKey_facebookInvite);
	it->setObject(CCString::create("%s ha inviato BRUSH."), kMyLocalKey_facebookWith);
	
	it->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
	it->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
	it->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
	it->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
	it->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
	it->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
	it->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
	
	it->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
	it->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
	it->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
	it->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
	it->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
	setObject(it, "it");
}

CCString* MyLocal::getLocalCode()
{
	string tempCode;
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
	NSString *currentLanguage = [languages objectAtIndex:0];
	
	// get the current language code.(such as English is "en", Chinese is "zh" and so on)
	NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
	NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];
	
	tempCode = [languageCode cStringUsingEncoding:NSASCIIStringEncoding];
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	tempCode = getCurrentLanguageJNI();
#endif
	
	return CCString::create(tempCode);
}

const char* MyLocal::getLocalForKey( MyLocalKey key )
{
	languageType = getLocalCode();
	CCArray* allkeys_ = this->allKeys();
	bool supportable = false;
	for(int i=0;i<allkeys_->count();i++)
	{
		CCString* key = (CCString*)allkeys_->objectAtIndex(i);
		if(languageType->isEqual(key))
		{
			supportable = true;
			break;
		}
	}
	if(!supportable)
	{
		languageType = CCString::create("en");
	}
	CCDictionary* localDic = (CCDictionary*)this->objectForKey(languageType->getCString());
	CCString* returnLocal = (CCString*)localDic->objectForKey(key);

	return returnLocal->getCString();
}

MyLocal* MyLocal::sharedInstance()
{
	static MyLocal* myLocal = NULL;
	if(myLocal == NULL)
	{
		myLocal = new MyLocal();
	}
	return myLocal;
}

void MyLocal::setLocal()
{
	languageType = CCString::create(getCurrentLanguageJNI());
}
