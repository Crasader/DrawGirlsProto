#ifndef DrawingJack_AudioEngine_h
#define DrawingJack_AudioEngine_h

#include	"cocos2d.h"
#include	"SimpleAudioEngine.h"
#include <algorithm>
#include <pthread.h>

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

class SoundEffect : public CCObject
{
public:
	string filename;
	unsigned int effectCode;
	bool cancut; // 중간에 잘릴 수 있는지...
	bool isLoop; // 루프중인가
private:
	
};

class AudioEngine : public CCObject
{
public:
	static AudioEngine* sharedInstance()
	{
		static AudioEngine* myAudio = NULL;
		if(myAudio == NULL)
		{
			myAudio = new AudioEngine();
		}
		return myAudio;
	}
	
	virtual ~AudioEngine()
	{
		soundEffects->release();
		SimpleAudioEngine::end();
	}
	
	void preloadSound(const char* filename)
	{
		mySAE->preloadBackgroundMusic(filename);
	}
	void playSound(const char* filename, bool loop)
	{
		if(playing_sound_name != filename)
		{
			playing_sound_name = filename;
			mySAE->playBackgroundMusic(filename, loop);
		}
	}
	void stopSound()
	{
		CCLOG("stop sound!! : %s", playing_sound_name.c_str());
		
		playing_sound_name = "";
		mySAE->stopBackgroundMusic(true);
	}
	
	void setSoundOnOff(bool t_b)
	{
		if(t_b)
		{
			mySAE->setBackgroundMusicVolume(0.3);
		}
		else
		{
			CCLOG("sound off!!");
			
			mySAE->setBackgroundMusicVolume(0.0);
		}
	}
	
	void setEffectOnOff(bool t_b)
	{
		if(!t_b)
			CCLOG("effect off!!");
		effectOn = t_b;
		mySAE->setEffectsVolume(1.f*(effectOn ? 1.f : 0)*(back_down ? 0 : 1.f));
	}
	
	void preloadGroanEffect(string t_type)
	{
		auto iter = find(preloaded_groan_list.begin(), preloaded_groan_list.end(), t_type);
		if(iter == preloaded_groan_list.end())
		{
			preloaded_groan_list.push_back(t_type);
			mySAE->preloadEffect(t_type.c_str());
		}
	}
	
	bool is_preloaded;
	
	void preloadThreadAction(int t_step)
	{
		is_thread_preloaded = false;
		pthread_t p_thread;
		int thr_id;
		thr_id = pthread_create(&p_thread, NULL, preload_function, (void *)t_step);
		if(thr_id < 0)
		{
			perror("thread create error : sound preload");
			exit(0);
		}
	}
	
	bool is_thread_preloaded;
	
	static void* preload_function(void* data)
	{
		do{
			AudioEngine::sharedInstance()->preloadEffectTitleStep((int)data);
		}while (0);
		
		AudioEngine::sharedInstance()->is_thread_preloaded = true;
		
		return NULL;
	}
	
	void preloadEffectTitleStep(int t_step)
	{
		if(is_preloaded)
			return;
		if(t_step == 1)
		{
			mySAE->preloadEffect("sound_gamestart.mp3");
			
			mySAE->preloadEffect("se_itemgacha.mp3");
			mySAE->preloadEffect("se_heartuse.mp3");
			mySAE->preloadEffect("se_pageturn.mp3");
			mySAE->preloadEffect("se_resultstar.mp3");
			mySAE->preloadEffect("se_clearreward.mp3");
			mySAE->preloadEffect("se_mapgacha.mp3");
			mySAE->preloadEffect("se_mapgacha_end.mp3");
			mySAE->preloadEffect("se_pieceget.mp3");
			mySAE->preloadEffect("se_pieceopen.mp3");
			mySAE->preloadEffect("se_puzzleopen_1.mp3");
			mySAE->preloadEffect("se_puzzleopen_2.mp3");
			mySAE->preloadEffect("se_lock.mp3");
			mySAE->preloadEffect("se_clock.mp3");
			mySAE->preloadEffect("se_cardget.mp3");
			mySAE->preloadEffect("se_piece.mp3");
			mySAE->preloadEffect("se_buy.mp3");
			mySAE->preloadEffect("se_button1.mp3");
			mySAE->preloadEffect("se_upgrade.mp3");
			mySAE->preloadEffect("bgm_gameover.mp3");
			mySAE->preloadEffect("bgm_gameclear.mp3");
			mySAE->preloadEffect("bgm_result.mp3");
			mySAE->preloadEffect("ment_rankup.mp3");
			mySAE->preloadEffect("ment_title.mp3");
			
			
			
			mySAE->preloadEffect("se_area.mp3");
			mySAE->preloadEffect("se_item.mp3");
			mySAE->preloadEffect("se_drawcancel.mp3");
			mySAE->preloadEffect("se_monattacked.mp3");
			mySAE->preloadEffect("se_energywave.mp3");
			mySAE->preloadEffect("se_castmissile.mp3");
			mySAE->preloadEffect("se_castspecial.mp3");
			mySAE->preloadEffect("se_castmap.mp3");
			mySAE->preloadEffect("se_subdie.mp3");
			
			mySAE->preloadEffect("se_destructionlaser_1.mp3");
			mySAE->preloadEffect("se_destructionlaser_2.mp3");
			mySAE->preloadEffect("se_missile.mp3");
			mySAE->preloadEffect("se_sun.mp3");
			mySAE->preloadEffect("se_slowzone.mp3");
			mySAE->preloadEffect("se_prison.mp3");
			mySAE->preloadEffect("se_instantmove.mp3");
			mySAE->preloadEffect("se_meteo_1.mp3");
			mySAE->preloadEffect("se_meteo_2.mp3");
			mySAE->preloadEffect("se_meteo_3.mp3");
			mySAE->preloadEffect("se_meteor.mp3");
			
			mySAE->preloadEffect("ment_30second.mp3");
			mySAE->preloadEffect("ment_99percent.mp3");
			mySAE->preloadEffect("ment_attack1.mp3");
			mySAE->preloadEffect("ment_attack2.mp3");
			mySAE->preloadEffect("ment_attack3.mp3");
			mySAE->preloadEffect("ment_attack4.mp3");
			mySAE->preloadEffect("ment_cast_crash.mp3");
			mySAE->preloadEffect("ment_cast_missile.mp3");
			mySAE->preloadEffect("ment_cast_special.mp3");
			mySAE->preloadEffect("ment_change_success.mp3");
			mySAE->preloadEffect("ment_die1.mp3");
			mySAE->preloadEffect("ment_die2.mp3");
			mySAE->preloadEffect("ment_die3.mp3");
			
			mySAE->preloadEffect("ment_gameover1.mp3");
			mySAE->preloadEffect("ment_gameover2.mp3");
			mySAE->preloadEffect("ment_gameover3.mp3");
			mySAE->preloadEffect("ment_go.mp3");
			mySAE->preloadEffect("ment_mission_fail.mp3");
			mySAE->preloadEffect("ment_mission_success.mp3");
			mySAE->preloadEffect("ment_pattern_cancel1.mp3");
			mySAE->preloadEffect("ment_pattern_cancel2.mp3");
			mySAE->preloadEffect("ment_pattern_cancel3.mp3");
			mySAE->preloadEffect("ment_pattern_cancel4.mp3");
			mySAE->preloadEffect("ment_ready.mp3");
			mySAE->preloadEffect("ment_resurrection1.mp3");
			mySAE->preloadEffect("ment_resurrection2.mp3");
			mySAE->preloadEffect("ment_spread_change.mp3");
			mySAE->preloadEffect("ment_stageclear1.mp3");
			mySAE->preloadEffect("ment_stageclear2.mp3");
			mySAE->preloadEffect("ment_stageclear3.mp3");
			mySAE->preloadEffect("ment_timeover.mp3");
			mySAE->preloadEffect("ment_unlockmap.mp3");
			
			mySAE->preloadEffect("sound_pil_life.mp3");
			mySAE->preloadEffect("sound_pil_plasma.mp3");
			mySAE->preloadEffect("sound_pil_fire.mp3");
			mySAE->preloadEffect("sound_pil_ice.mp3");
			mySAE->preloadEffect("sound_pil_lightning.mp3");
			mySAE->preloadEffect("sound_pil_wind.mp3");
			mySAE->preloadEffect("sound_ready.mp3");
			mySAE->preloadEffect("sound_go.mp3");
			
			mySAE->preloadEffect("sound_get_coin.mp3");
			mySAE->preloadEffect("sound_basic_missile_shoot.mp3");
			mySAE->preloadEffect("sound_spin_missile.mp3");
			mySAE->preloadEffect("sound_jack_die.mp3");
			mySAE->preloadEffect("sound_jack_drawing.mp3");
			mySAE->preloadEffect("sound_casting_attack.mp3");
			mySAE->preloadEffect("sound_casting_crash.mp3");
			mySAE->preloadEffect("sound_casting_option.mp3");
			mySAE->preloadEffect("sound_stamp.mp3");
			mySAE->preloadEffect("sound_jack_basic_missile_shoot.mp3");
			mySAE->preloadEffect("sound_jack_special_missile.mp3");
			mySAE->preloadEffect("sound_jack_missile_bomb.mp3");
			mySAE->preloadEffect("sound_crashed_map.mp3");
		}
		else if(t_step == 2)
		{
			mySAE->preloadEffect("sound_pilsalgi.mp3");
			mySAE->preloadEffect("sound_bomb_wave.mp3");
			mySAE->preloadEffect("sound_throw_obj_shot.mp3");
			mySAE->preloadEffect("sound_threecusion_bomb.mp3");
			mySAE->preloadEffect("sound_rock_falling.mp3");
			mySAE->preloadEffect("sound_fire_shot.mp3");
			mySAE->preloadEffect("sound_stun_hit.mp3");
			mySAE->preloadEffect("sound_teleport.mp3");
			mySAE->preloadEffect("sound_time_noti.mp3");
			mySAE->preloadEffect("sound_ice_hold.mp3");
			mySAE->preloadEffect("sound_returntojack.mp3");
			mySAE->preloadEffect("sound_sleep.mp3");
			mySAE->preloadEffect("sound_barrier_pass.mp3");
			mySAE->preloadEffect("sound_angle_beem.mp3");
			mySAE->preloadEffect("sound_tickingTimeBomb.mp3");
			mySAE->preloadEffect("sound_meteor.mp3");
			mySAE->preloadEffect("sound_TGshoot.mp3");
			mySAE->preloadEffect("sound_satellitebeam.mp3");
			
			mySAE->preloadEffect("sound_attackpattern_base.mp3");
			mySAE->preloadEffect("sound_attackpattern_crash.mp3");
			mySAE->preloadEffect("sound_bomb_common.mp3");
			mySAE->preloadEffect("sound_clear_bgm.mp3");
			mySAE->preloadEffect("sound_clear_ment.mp3");
			mySAE->preloadEffect("sound_die_jack.mp3");
			mySAE->preloadEffect("sound_gameover_bgm.mp3");
			mySAE->preloadEffect("sound_gameover_ment.mp3");
			mySAE->preloadEffect("sound_showtime_bgm.mp3");
			mySAE->preloadEffect("sound_showtime_ment.mp3");
		}
		else if(t_step == 3)
		{
			mySAE->preloadEffect("sound_ending_clear.mp3");
			mySAE->preloadEffect("sound_ending_fail.mp3");
			mySAE->preloadEffect("sound_star_visible.mp3");
			mySAE->preloadEffect("sound_calc.mp3");
			
			
			mySAE->preloadEffect("groan1.mp3");
			mySAE->preloadEffect("groan2.mp3");
			mySAE->preloadEffect("groan3.mp3");
			mySAE->preloadEffect("groan4.mp3");
			mySAE->preloadEffect("groan5.mp3");
			mySAE->preloadEffect("groan6.mp3");
			mySAE->preloadEffect("groan7.mp3");
			mySAE->preloadEffect("groan8.mp3");
			mySAE->preloadEffect("groan9.mp3");
			
			mySAE->preloadEffect("sg_mg_win1.mp3");
			mySAE->preloadEffect("sg_mg_win2.mp3");
			mySAE->preloadEffect("sg_mg_win3.mp3");
			mySAE->preloadEffect("sg_mg_draw.mp3");
			mySAE->preloadEffect("sg_mg_fail.mp3");
			mySAE->preloadEffect("sg_mg_light.mp3");
			
			mySAE->preloadEffect("se_typing_1.mp3");
			mySAE->preloadEffect("se_typing_2.mp3");
			mySAE->preloadEffect("se_typing_3.mp3");
			mySAE->preloadEffect("se_typing_4.mp3");
			mySAE->preloadEffect("se_typing_5.mp3");
			
			is_preloaded = true;
		}
	}
	
	void preloadEffectScene(string scenename)
	{
		if(scenename == "Title" && !is_preloaded)
		{
			is_preloaded = true;
//			mySAE->preloadEffect("sound_buttonClick_Low.mp3");
//			mySAE->preloadEffect("sound_scrolling.mp3");
//			mySAE->preloadEffect("sound_start_brush.mp3");
//			mySAE->preloadEffect("sound_shuttermove_start.m4a");
//			mySAE->preloadEffect("sound_shutter_closed.m4a");
			
			mySAE->preloadEffect("sound_gamestart.mp3");
			
			mySAE->preloadEffect("se_itemgacha.mp3");
			mySAE->preloadEffect("se_heartuse.mp3");
			mySAE->preloadEffect("se_pageturn.mp3");
			mySAE->preloadEffect("se_resultstar.mp3");
			mySAE->preloadEffect("se_clearreward.mp3");
			mySAE->preloadEffect("se_mapgacha.mp3");
			mySAE->preloadEffect("se_mapgacha_end.mp3");
			mySAE->preloadEffect("se_pieceget.mp3");
			mySAE->preloadEffect("se_pieceopen.mp3");
			mySAE->preloadEffect("se_puzzleopen_1.mp3");
			mySAE->preloadEffect("se_puzzleopen_2.mp3");
			mySAE->preloadEffect("se_lock.mp3");
			mySAE->preloadEffect("se_clock.mp3");
			mySAE->preloadEffect("se_cardget.mp3");
			mySAE->preloadEffect("se_piece.mp3");
			mySAE->preloadEffect("se_buy.mp3");
			mySAE->preloadEffect("se_button1.mp3");
			mySAE->preloadEffect("se_upgrade.mp3");
			mySAE->preloadEffect("bgm_gameover.mp3");
			mySAE->preloadEffect("bgm_gameclear.mp3");
			mySAE->preloadEffect("bgm_result.mp3");
			mySAE->preloadEffect("ment_rankup.mp3");
			mySAE->preloadEffect("ment_title.mp3");
			
			
			
			mySAE->preloadEffect("se_area.mp3");
			mySAE->preloadEffect("se_item.mp3");
			mySAE->preloadEffect("se_drawcancel.mp3");
			mySAE->preloadEffect("se_monattacked.mp3");
			mySAE->preloadEffect("se_energywave.mp3");
			mySAE->preloadEffect("se_castmissile.mp3");
			mySAE->preloadEffect("se_castspecial.mp3");
			mySAE->preloadEffect("se_castmap.mp3");
			mySAE->preloadEffect("se_subdie.mp3");
			
			mySAE->preloadEffect("se_destructionlaser_1.mp3");
			mySAE->preloadEffect("se_destructionlaser_2.mp3");
			mySAE->preloadEffect("se_missile.mp3");
			mySAE->preloadEffect("se_sun.mp3");
			mySAE->preloadEffect("se_slowzone.mp3");
			mySAE->preloadEffect("se_prison.mp3");
			mySAE->preloadEffect("se_instantmove.mp3");
			mySAE->preloadEffect("se_meteo_1.mp3");
			mySAE->preloadEffect("se_meteo_2.mp3");
			mySAE->preloadEffect("se_meteo_3.mp3");
			mySAE->preloadEffect("se_meteor.mp3");
			
			mySAE->preloadEffect("ment_30second.mp3");
			mySAE->preloadEffect("ment_99percent.mp3");
			mySAE->preloadEffect("ment_attack1.mp3");
			mySAE->preloadEffect("ment_attack2.mp3");
			mySAE->preloadEffect("ment_attack3.mp3");
			mySAE->preloadEffect("ment_attack4.mp3");
			mySAE->preloadEffect("ment_cast_crash.mp3");
			mySAE->preloadEffect("ment_cast_missile.mp3");
			mySAE->preloadEffect("ment_cast_special.mp3");
			mySAE->preloadEffect("ment_change_success.mp3");
			mySAE->preloadEffect("ment_die1.mp3");
			mySAE->preloadEffect("ment_die2.mp3");
			mySAE->preloadEffect("ment_die3.mp3");
			mySAE->preloadEffect("ment_gameover1.mp3");
			mySAE->preloadEffect("ment_gameover2.mp3");
			mySAE->preloadEffect("ment_gameover3.mp3");
			mySAE->preloadEffect("ment_go.mp3");
			mySAE->preloadEffect("ment_mission_fail.mp3");
			mySAE->preloadEffect("ment_mission_success.mp3");
			mySAE->preloadEffect("ment_pattern_cancel1.mp3");
			mySAE->preloadEffect("ment_pattern_cancel2.mp3");
			mySAE->preloadEffect("ment_pattern_cancel3.mp3");
			mySAE->preloadEffect("ment_pattern_cancel4.mp3");
			mySAE->preloadEffect("ment_ready.mp3");
			mySAE->preloadEffect("ment_resurrection1.mp3");
			mySAE->preloadEffect("ment_resurrection2.mp3");
			mySAE->preloadEffect("ment_spread_change.mp3");
			mySAE->preloadEffect("ment_stageclear1.mp3");
			mySAE->preloadEffect("ment_stageclear2.mp3");
			mySAE->preloadEffect("ment_stageclear3.mp3");
			mySAE->preloadEffect("ment_timeover.mp3");
			mySAE->preloadEffect("ment_unlockmap.mp3");
			
			mySAE->preloadEffect("sound_ready.mp3");
			mySAE->preloadEffect("sound_go.mp3");
			mySAE->preloadEffect("sound_get_coin.mp3");
			mySAE->preloadEffect("sound_basic_missile_shoot.mp3");
			mySAE->preloadEffect("sound_spin_missile.mp3");
			mySAE->preloadEffect("sound_jack_die.mp3");
			mySAE->preloadEffect("sound_jack_drawing.mp3");
			mySAE->preloadEffect("sound_casting_attack.mp3");
			mySAE->preloadEffect("sound_casting_crash.mp3");
			mySAE->preloadEffect("sound_casting_option.mp3");
			mySAE->preloadEffect("sound_stamp.mp3");
			mySAE->preloadEffect("sound_jack_basic_missile_shoot.mp3");
			mySAE->preloadEffect("sound_jack_special_missile.mp3");
			mySAE->preloadEffect("sound_jack_missile_bomb.mp3");
			mySAE->preloadEffect("sound_crashed_map.mp3");
			mySAE->preloadEffect("sound_pilsalgi.mp3");
			mySAE->preloadEffect("sound_bomb_wave.mp3");
			mySAE->preloadEffect("sound_throw_obj_shot.mp3");
			mySAE->preloadEffect("sound_threecusion_bomb.mp3");
			mySAE->preloadEffect("sound_rock_falling.mp3");
			mySAE->preloadEffect("sound_fire_shot.mp3");
			mySAE->preloadEffect("sound_stun_hit.mp3");
			mySAE->preloadEffect("sound_teleport.mp3");
			mySAE->preloadEffect("sound_time_noti.mp3");
			mySAE->preloadEffect("sound_ice_hold.mp3");
			mySAE->preloadEffect("sound_returntojack.mp3");
			mySAE->preloadEffect("sound_sleep.mp3");
			mySAE->preloadEffect("sound_pil_life.mp3");
			mySAE->preloadEffect("sound_pil_plasma.mp3");
			mySAE->preloadEffect("sound_pil_fire.mp3");
			mySAE->preloadEffect("sound_pil_ice.mp3");
			mySAE->preloadEffect("sound_pil_lightning.mp3");
			mySAE->preloadEffect("sound_pil_wind.mp3");
			mySAE->preloadEffect("sound_barrier_pass.mp3");
			mySAE->preloadEffect("sound_angle_beem.mp3");
			mySAE->preloadEffect("sound_tickingTimeBomb.mp3");
			mySAE->preloadEffect("sound_meteor.mp3");
			mySAE->preloadEffect("sound_TGshoot.mp3");
			mySAE->preloadEffect("sound_satellitebeam.mp3");
			
			mySAE->preloadEffect("sound_attackpattern_base.mp3");
			mySAE->preloadEffect("sound_attackpattern_crash.mp3");
			mySAE->preloadEffect("sound_bomb_common.mp3");
			mySAE->preloadEffect("sound_clear_bgm.mp3");
			mySAE->preloadEffect("sound_clear_ment.mp3");
			mySAE->preloadEffect("sound_die_jack.mp3");
			mySAE->preloadEffect("sound_gameover_bgm.mp3");
			mySAE->preloadEffect("sound_gameover_ment.mp3");
			mySAE->preloadEffect("sound_showtime_bgm.mp3");
			mySAE->preloadEffect("sound_showtime_ment.mp3");
			
			
			mySAE->preloadEffect("sound_ending_clear.mp3");
			mySAE->preloadEffect("sound_ending_fail.mp3");
			mySAE->preloadEffect("sound_star_visible.mp3");
			mySAE->preloadEffect("sound_calc.mp3");
			
//			mySAE->preloadEffect("groan1.mp3");
//			mySAE->preloadEffect("groan2.mp3");
//			mySAE->preloadEffect("groan3.mp3");
//			mySAE->preloadEffect("groan4.mp3");
//			mySAE->preloadEffect("groan5.mp3");
//			mySAE->preloadEffect("groan6.mp3");
//			mySAE->preloadEffect("groan7.mp3");
//			mySAE->preloadEffect("groan8.mp3");
//			mySAE->preloadEffect("groan9.mp3");
//			mySAE->preloadEffect("groan10.mp3");
//			mySAE->preloadEffect("groan11.mp3");
//			mySAE->preloadEffect("groan12.mp3");
//			mySAE->preloadEffect("groan13.mp3");
//			mySAE->preloadEffect("groan14.mp3");
//			mySAE->preloadEffect("groan15.mp3");
//			mySAE->preloadEffect("groan16.mp3");
//			mySAE->preloadEffect("groan17.mp3");
//			mySAE->preloadEffect("groan18.mp3");
//			mySAE->preloadEffect("groan19.mp3");
//			mySAE->preloadEffect("groan20.mp3");
//			mySAE->preloadEffect("groan21.mp3");
//			mySAE->preloadEffect("groan22.mp3");
//			mySAE->preloadEffect("groan23.mp3");
//			mySAE->preloadEffect("groan24.mp3");
//			mySAE->preloadEffect("groan25.mp3");
//			mySAE->preloadEffect("groan26.mp3");
//			mySAE->preloadEffect("groan27.mp3");
//			mySAE->preloadEffect("groan28.mp3");
//			mySAE->preloadEffect("groan29.mp3");
			
			mySAE->preloadEffect("groan1.mp3");
			mySAE->preloadEffect("groan2.mp3");
			mySAE->preloadEffect("groan3.mp3");
			mySAE->preloadEffect("groan4.mp3");
			mySAE->preloadEffect("groan5.mp3");
			mySAE->preloadEffect("groan6.mp3");
			mySAE->preloadEffect("groan7.mp3");
			mySAE->preloadEffect("groan8.mp3");
			mySAE->preloadEffect("groan9.mp3");
			
			mySAE->preloadEffect("se_typing_1.mp3");
			mySAE->preloadEffect("se_typing_2.mp3");
			mySAE->preloadEffect("se_typing_3.mp3");
			mySAE->preloadEffect("se_typing_4.mp3");
			
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
			
		}
		else if(scenename == "Maingame")
		{
			
//			mySAE->preloadEffect("ment_.mp3");
			
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
//			mySAE->preloadEffect(".mp3");
			
			
			
			
		}
		else if(scenename == "Ending")
		{
			
		}
		else if(scenename == "playtutorial")
		{
			mySAE->preloadEffect("ment_tutorial_start.mp3");
			mySAE->preloadEffect("ment_tutorial1.mp3");
			mySAE->preloadEffect("ment_tutorial2.mp3");
			mySAE->preloadEffect("ment_tutorial3.mp3");
			mySAE->preloadEffect("ment_tutorial4.mp3");
			mySAE->preloadEffect("ment_tutorial5.mp3");
			mySAE->preloadEffect("ment_tutorial6.mp3");
			mySAE->preloadEffect("ment_tutorial7.mp3");
			mySAE->preloadEffect("ment_tutorial8.mp3");
			mySAE->preloadEffect("ment_tutorial9.mp3");
			mySAE->preloadEffect("ment_tutorial10.mp3");
			mySAE->preloadEffect("ment_tutorial11.mp3");
			mySAE->preloadEffect("ment_tutorial12.mp3");
		}
	}
	void unloadEffectScene(string scenename)
	{
		// 전부 멈추는 코드
		while(soundEffects->count() > 0)
		{
			SoundEffect* target = (SoundEffect*)soundEffects->lastObject();
			mySAE->stopEffect(target->effectCode);
			soundEffects->removeObject(target);
		}
		
		if(scenename == "Title")
		{
			
		}
		else if(scenename == "Maingame")
		{
//			mySAE->unloadEffect("se_area.mp3");
//			mySAE->unloadEffect("se_item.mp3");
//			mySAE->unloadEffect("se_drawcancel.mp3");
//			mySAE->unloadEffect("se_monattacked.mp3");
//			mySAE->unloadEffect("se_energywave.mp3");
//			mySAE->unloadEffect("se_castmissile.mp3");
//			mySAE->unloadEffect("se_castspecial.mp3");
//			mySAE->unloadEffect("se_castmap.mp3");
//			mySAE->unloadEffect("se_subdie.mp3");
//			
//			mySAE->unloadEffect("se_destructionlaser_1.mp3");
//			mySAE->unloadEffect("se_destructionlaser_2.mp3");
//			mySAE->unloadEffect("se_missile.mp3");
//			mySAE->unloadEffect("se_sun.mp3");
//			mySAE->unloadEffect("se_slowzone.mp3");
//			mySAE->unloadEffect("se_prison.mp3");
//			mySAE->unloadEffect("se_instantmove.mp3");
//			mySAE->unloadEffect("se_meteo_1.mp3");
//			mySAE->unloadEffect("se_meteo_2.mp3");
//			mySAE->unloadEffect("se_meteo_3.mp3");
//			mySAE->unloadEffect("se_meteor.mp3");
//			
//			mySAE->unloadEffect("ment_30second.mp3");
//			mySAE->unloadEffect("ment_99percent.mp3");
//			mySAE->unloadEffect("ment_attack1.mp3");
//			mySAE->unloadEffect("ment_attack2.mp3");
//			mySAE->unloadEffect("ment_attack3.mp3");
//			mySAE->unloadEffect("ment_attack4.mp3");
//			mySAE->unloadEffect("ment_cast_crash.mp3");
//			mySAE->unloadEffect("ment_cast_missile.mp3");
//			mySAE->unloadEffect("ment_cast_special.mp3");
//			mySAE->unloadEffect("ment_change_success.mp3");
//			mySAE->unloadEffect("ment_die1.mp3");
//			mySAE->unloadEffect("ment_die2.mp3");
//			mySAE->unloadEffect("ment_die3.mp3");
//			mySAE->unloadEffect("ment_gameover1.mp3");
//			mySAE->unloadEffect("ment_gameover2.mp3");
//			mySAE->unloadEffect("ment_gameover3.mp3");
//			mySAE->unloadEffect("ment_go.mp3");
//			mySAE->unloadEffect("ment_mission_fail.mp3");
//			mySAE->unloadEffect("ment_mission_success.mp3");
//			mySAE->unloadEffect("ment_pattern_cancel1.mp3");
//			mySAE->unloadEffect("ment_pattern_cancel2.mp3");
//			mySAE->unloadEffect("ment_pattern_cancel3.mp3");
//			mySAE->unloadEffect("ment_pattern_cancel4.mp3");
//			mySAE->unloadEffect("ment_ready.mp3");
//			mySAE->unloadEffect("ment_resurrection1.mp3");
//			mySAE->unloadEffect("ment_resurrection2.mp3");
//			mySAE->unloadEffect("ment_spread_change.mp3");
//			mySAE->unloadEffect("ment_stageclear1.mp3");
//			mySAE->unloadEffect("ment_stageclear2.mp3");
//			mySAE->unloadEffect("ment_stageclear3.mp3");
//			mySAE->unloadEffect("ment_timeover.mp3");
//			mySAE->unloadEffect("ment_unlockmap.mp3");
//			
//			mySAE->unloadEffect("sound_ready.mp3");
//			mySAE->unloadEffect("sound_go.mp3");
//			mySAE->unloadEffect("sound_get_coin.mp3");
//			mySAE->unloadEffect("sound_basic_missile_shoot.mp3");
//			mySAE->unloadEffect("sound_spin_missile.mp3");
//			mySAE->unloadEffect("sound_jack_die.mp3");
//			mySAE->unloadEffect("sound_jack_drawing.mp3");
//			mySAE->unloadEffect("sound_casting_attack.mp3");
//			mySAE->unloadEffect("sound_casting_crash.mp3");
//			mySAE->unloadEffect("sound_casting_option.mp3");
//			mySAE->unloadEffect("sound_stamp.mp3");
//			mySAE->unloadEffect("sound_jack_basic_missile_shoot.mp3");
//			mySAE->unloadEffect("sound_jack_special_missile.mp3");
//			mySAE->unloadEffect("sound_jack_missile_bomb.mp3");
//			mySAE->unloadEffect("sound_crashed_map.mp3");
//			mySAE->unloadEffect("sound_pilsalgi.mp3");
//			mySAE->unloadEffect("sound_bomb_wave.mp3");
//			mySAE->unloadEffect("sound_throw_obj_shot.mp3");
//			mySAE->unloadEffect("sound_threecusion_bomb.mp3");
//			mySAE->unloadEffect("sound_rock_falling.mp3");
//			mySAE->unloadEffect("sound_fire_shot.mp3");
//			mySAE->unloadEffect("sound_stun_hit.mp3");
//			mySAE->unloadEffect("sound_teleport.mp3");
//			mySAE->unloadEffect("sound_time_noti.mp3");
//			mySAE->unloadEffect("sound_ice_hold.mp3");
//			mySAE->unloadEffect("sound_returntojack.mp3");
//			mySAE->unloadEffect("sound_sleep.mp3");
//			mySAE->unloadEffect("sound_pil_life.mp3");
//			mySAE->unloadEffect("sound_pil_plasma.mp3");
//			mySAE->unloadEffect("sound_pil_fire.mp3");
//			mySAE->unloadEffect("sound_pil_ice.mp3");
//			mySAE->unloadEffect("sound_pil_lightning.mp3");
//			mySAE->unloadEffect("sound_pil_wind.mp3");
//			mySAE->unloadEffect("sound_barrier_pass.mp3");
//			mySAE->unloadEffect("sound_angle_beem.mp3");
//			mySAE->unloadEffect("sound_tickingTimeBomb.mp3");
//			mySAE->unloadEffect("sound_meteor.mp3");
//			mySAE->unloadEffect("sound_TGshoot.mp3");
//			mySAE->unloadEffect("sound_satellitebeam.mp3");
//			
//			mySAE->unloadEffect("sound_attackpattern_base.mp3");
//			mySAE->unloadEffect("sound_attackpattern_crash.mp3");
//			mySAE->unloadEffect("sound_bomb_common.mp3");
////			mySAE->unloadEffect("sound_clear_bgm.mp3");
//			mySAE->unloadEffect("sound_clear_ment.mp3");
//			mySAE->unloadEffect("sound_die_jack.mp3");
//			mySAE->unloadEffect("sound_gameover_bgm.mp3");
//			mySAE->unloadEffect("sound_gameover_ment.mp3");
//			mySAE->unloadEffect("sound_showtime_bgm.mp3");
//			mySAE->unloadEffect("sound_showtime_ment.mp3");
		}
		else if(scenename == "Ending")
		{
//			mySAE->unloadEffect("sound_ending_clear.mp3");
//			mySAE->unloadEffect("sound_ending_fail.mp3");
//			mySAE->unloadEffect("sound_star_visible.mp3");
//			mySAE->unloadEffect("sound_calc.mp3");
		}
		else if(scenename == "playtutorial")
		{
			mySAE->unloadEffect("ment_tutorial_start.mp3");
			mySAE->unloadEffect("ment_tutorial1.mp3");
			mySAE->unloadEffect("ment_tutorial2.mp3");
			mySAE->unloadEffect("ment_tutorial3.mp3");
			mySAE->unloadEffect("ment_tutorial4.mp3");
			mySAE->unloadEffect("ment_tutorial5.mp3");
			mySAE->unloadEffect("ment_tutorial6.mp3");
			mySAE->unloadEffect("ment_tutorial7.mp3");
			mySAE->unloadEffect("ment_tutorial8.mp3");
			mySAE->unloadEffect("ment_tutorial9.mp3");
			mySAE->unloadEffect("ment_tutorial10.mp3");
			mySAE->unloadEffect("ment_tutorial11.mp3");
			mySAE->unloadEffect("ment_tutorial12.mp3");
		}
	}
	void startGame()
	{
		soundEffects->removeAllObjects();
	}
	
	void playGroanEffect(string filename)
	{
		playEffect(filename.c_str(), false, true);
	}
	
	void playEffect(const char* filename, bool loop = false, bool cancut = false)
	{
		if(effectOn == false)
			return;
		
		if(back_down)
			CCLOG("will not play effect. back down!!");
		
		// loop 와 cancut 이 둘다 false 여야 그냥 재생만.
		// 아니면은 일단 저장해야 됨.
		// loop = false && cancut == true 일 때,
		// 그냥 재생하고 저장함.
		// loop = true && cancut == false 일 때,
		// 재생하고 있는게 있다면 무시하고, 저장하고 재생
		// loop = true && cancut == true 이면,
		// cancut == false 인거랑 동일하게 처리하면 될 듯.
		//
		if(!loop && !cancut)
		{
			mySAE->playEffect(filename);
		}
		if(loop)
		{
			bool is_ing = false;
			for(int i=0;i<soundEffects->count();i++)
			{
				SoundEffect* target = (SoundEffect*)soundEffects->objectAtIndex(i);
				if(target->filename == filename && target->isLoop)
				{
					is_ing = true;
					break;
				}
			}

			if(!is_ing)
			{
				string real_filename = filename;
//				CCLOG("PlayEffect loop : %s", filename);

				SoundEffect* t_effect = new SoundEffect();
				t_effect->autorelease();
				unsigned int code = mySAE->playEffect(real_filename.c_str(), true);
				t_effect->filename = filename;
				t_effect->effectCode = code;
				t_effect->isLoop = true;
				t_effect->cancut = false;
				soundEffects->addObject(t_effect);
			}
		}

		if(!loop && cancut)
		{
			stopEffectForCutting(); // 잘릴 수 있는 애들은 다 자름			

			string real_filename = filename;
			SoundEffect* t_effect = new SoundEffect();
			t_effect->autorelease();
			unsigned int code = mySAE->playEffect(real_filename.c_str(), false);
			t_effect->filename = filename;
			t_effect->effectCode = code;
			t_effect->isLoop = false;
			t_effect->cancut = true;
			soundEffects->addObject(t_effect);
		}
	}

	// 중간에 잘릴 수 있는 애들은 자름
	void stopEffectForCutting()
	{
		std::vector<SoundEffect*> removingObject;
		for(int i=0; i<soundEffects->count(); i++)
		{
			SoundEffect* target = (SoundEffect*)soundEffects->objectAtIndex(i);
			if(target->cancut)
			{
				removingObject.push_back(target);
			}
		}

		for(auto i : removingObject)
		{
			mySAE->stopEffect(i->effectCode);
			soundEffects->removeObject(i);
		}
	}
	void stopEffect(const char* filename)
	{
//		CCLOG("StopEffect loop check start : %s", filename);
		bool is_found;
		do
		{
			is_found = false;
			for(int i=0;i<soundEffects->count();i++)
			{
				SoundEffect* target = (SoundEffect*)soundEffects->objectAtIndex(i);
				if(target->filename == filename)
				{
//					CCLOG("StopEffect loop find : %s", filename);
					is_found = true;
					mySAE->stopEffect(target->effectCode);
					soundEffects->removeObject(target);
					break;
				}
			}
		}while(is_found);
	}
	
	void stopAllEffects()
	{
		while(soundEffects->count() > 0)
		{
			SoundEffect* target = (SoundEffect*)soundEffects->objectAtIndex(0);
			CCLOG("StopEffect loop find : %s", target->filename.c_str());
			mySAE->stopEffect(target->effectCode);
			soundEffects->removeObject(target);
		}
		
		CCLOG("StopAllEffect loop");
		mySAE->stopAllEffects();
	}
	
	void initMusicOnOff(int t_num)
	{
		musicOn = t_num;
		if(musicOn == 0)
		{
			setSoundOnOff(true);
			setEffectOnOff(true);
		}
		else if(musicOn == 1)
		{
			setSoundOnOff(false);
			setEffectOnOff(true);
		}
		else
		{
			setSoundOnOff(false);
			setEffectOnOff(false);
		}
	}
	
	int setMusicOnOff()
	{
		musicOn = (musicOn + 1)%3;
		if(musicOn == 0)
		{
			setSoundOnOff(true);
			setEffectOnOff(true);
		}
		else if(musicOn == 1)
		{
			setSoundOnOff(false);
			setEffectOnOff(true);
		}
		else
		{
			setSoundOnOff(false);
			setEffectOnOff(false);
		}
		return musicOn;
	}
	
	int getIsOnMusic()
	{
		return musicOn;
	}
	
	void setAppBack()
	{
		back_down = true;
		mySAE->setEffectsVolume(1.f*(effectOn ? 1.f : 0)*(back_down ? 0 : 1.f));
	}
	
	void setAppFore()
	{
		back_down = false;
		mySAE->setEffectsVolume(1.f*(effectOn ? 1.f : 0)*(back_down ? 0 : 1.f));
	}
	
private:
	SimpleAudioEngine* mySAE;
	CCArray* soundEffects;
	string playing_sound_name;
	int musicOn;
	bool effectOn;
	vector<string> preloaded_groan_list;
	bool back_down;
	
	AudioEngine() : CCObject()
	{
		mySAE = SimpleAudioEngine::sharedEngine();
		back_down = false;
		is_preloaded = false;
		soundEffects = new CCArray(1);
		preloaded_groan_list.clear();
	}
	
};

#endif
