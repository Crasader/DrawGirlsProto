#ifndef DrawingJack_AudioEngine_h
#define DrawingJack_AudioEngine_h

#include	"cocos2d.h"
#include	"SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

class LoopEffect : public CCObject
{
public:
	string filename;
	unsigned int effectCode;
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
		loopEffects->release();
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
		playing_sound_name = "";
		mySAE->stopBackgroundMusic();
	}
	
	void setSoundOnOff(bool t_b)
	{
		if(t_b)
		{
			mySAE->setBackgroundMusicVolume(1.0);
		}
		else
		{
			mySAE->setBackgroundMusicVolume(0.0);
		}
	}
	
	void setEffectOnOff(bool t_b)
	{
		effectOn = t_b;
	}
	
	void preloadEffectScene(string scenename)
	{
		if(scenename == "Title")
		{
//			mySAE->preloadEffect("sound_buttonClick_Low.mp3");
//			mySAE->preloadEffect("sound_scrolling.mp3");
//			mySAE->preloadEffect("sound_start_brush.mp3");
//			mySAE->preloadEffect("sound_shuttermove_start.m4a");
//			mySAE->preloadEffect("sound_shutter_closed.m4a");
			
			mySAE->preloadEffect("sound_gamestart.mp3");
		}
		else if(scenename == "Maingame")
		{
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
			mySAE->preloadEffect("sound_casting_attack.ogg");
			mySAE->preloadEffect("sound_casting_crash.ogg");
			mySAE->preloadEffect("sound_casting_option.ogg");
			mySAE->preloadEffect("sound_stamp.mp3");
			mySAE->preloadEffect("sound_jack_basic_missile_shoot.mp3");
			mySAE->preloadEffect("sound_jack_special_missile.mp3");
			mySAE->preloadEffect("sound_jack_missile_bomb.mp3");
			mySAE->preloadEffect("sound_crashed_map.mp3");
			mySAE->preloadEffect("sound_pilsalgi.mp3");
			mySAE->preloadEffect("sound_bomb_wave.mp3");
			mySAE->preloadEffect("sound_bomb_wave.ogg");
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
		}
		else if(scenename == "Ending")
		{
			mySAE->preloadEffect("sound_ending_clear.mp3");
			mySAE->preloadEffect("sound_ending_fail.mp3");
			mySAE->preloadEffect("sound_star_visible.mp3");
			mySAE->preloadEffect("sound_calc.mp3");
		}
	}
	void unloadEffectScene(string scenename)
	{
		if(scenename == "Title")
		{
			
		}
		else if(scenename == "Maingame")
		{
			mySAE->unloadEffect("sound_ready.mp3");
			mySAE->unloadEffect("sound_go.mp3");
			mySAE->unloadEffect("sound_get_coin.mp3");
			mySAE->unloadEffect("sound_basic_missile_shoot.mp3");
			mySAE->unloadEffect("sound_spin_missile.mp3");
			mySAE->unloadEffect("sound_jack_die.mp3");
			mySAE->unloadEffect("sound_jack_drawing.mp3");
			mySAE->unloadEffect("sound_casting_attack.mp3");
			mySAE->unloadEffect("sound_casting_crash.mp3");
			mySAE->unloadEffect("sound_casting_option.mp3");
			mySAE->unloadEffect("sound_casting_attack.ogg");
			mySAE->unloadEffect("sound_casting_crash.ogg");
			mySAE->unloadEffect("sound_casting_option.ogg");
			mySAE->unloadEffect("sound_stamp.mp3");
			mySAE->unloadEffect("sound_jack_basic_missile_shoot.mp3");
			mySAE->unloadEffect("sound_jack_special_missile.mp3");
			mySAE->unloadEffect("sound_jack_missile_bomb.mp3");
			mySAE->unloadEffect("sound_crashed_map.mp3");
			mySAE->unloadEffect("sound_pilsalgi.mp3");
			mySAE->unloadEffect("sound_bomb_wave.mp3");
			mySAE->unloadEffect("sound_bomb_wave.ogg");
			mySAE->unloadEffect("sound_throw_obj_shot.mp3");
			mySAE->unloadEffect("sound_threecusion_bomb.mp3");
			mySAE->unloadEffect("sound_rock_falling.mp3");
			mySAE->unloadEffect("sound_fire_shot.mp3");
			mySAE->unloadEffect("sound_stun_hit.mp3");
			mySAE->unloadEffect("sound_teleport.mp3");
			mySAE->unloadEffect("sound_time_noti.mp3");
			mySAE->unloadEffect("sound_ice_hold.mp3");
			mySAE->unloadEffect("sound_returntojack.mp3");
			mySAE->unloadEffect("sound_sleep.mp3");
			mySAE->unloadEffect("sound_pil_life.mp3");
			mySAE->unloadEffect("sound_pil_plasma.mp3");
			mySAE->unloadEffect("sound_pil_fire.mp3");
			mySAE->unloadEffect("sound_pil_ice.mp3");
			mySAE->unloadEffect("sound_pil_lightning.mp3");
			mySAE->unloadEffect("sound_pil_wind.mp3");
			mySAE->unloadEffect("sound_barrier_pass.mp3");
			mySAE->unloadEffect("sound_angle_beem.mp3");
			mySAE->unloadEffect("sound_tickingTimeBomb.mp3");
			mySAE->unloadEffect("sound_meteor.mp3");
			mySAE->unloadEffect("sound_TGshoot.mp3");
			mySAE->unloadEffect("sound_satellitebeam.mp3");
			
			mySAE->unloadEffect("sound_attackpattern_base.mp3");
			mySAE->unloadEffect("sound_attackpattern_crash.mp3");
			mySAE->unloadEffect("sound_bomb_common.mp3");
//			mySAE->unloadEffect("sound_clear_bgm.mp3");
			mySAE->unloadEffect("sound_clear_ment.mp3");
			mySAE->unloadEffect("sound_die_jack.mp3");
			mySAE->unloadEffect("sound_gameover_bgm.mp3");
			mySAE->unloadEffect("sound_gameover_ment.mp3");
			mySAE->unloadEffect("sound_showtime_bgm.mp3");
			mySAE->unloadEffect("sound_showtime_ment.mp3");
		}
		else if(scenename == "Ending")
		{
			mySAE->unloadEffect("sound_ending_clear.mp3");
			mySAE->unloadEffect("sound_ending_fail.mp3");
			mySAE->unloadEffect("sound_star_visible.mp3");
			mySAE->unloadEffect("sound_calc.mp3");
		}
	}
	void startGame()
	{
		loopEffects->removeAllObjects();
	}
	void playEffect(const char* filename, bool loop)
	{
		if(effectOn)
		{
			if(loop)
			{
				bool is_ing = false;
				for(int i=0;i<loopEffects->count();i++)
				{
					LoopEffect* target = (LoopEffect*)loopEffects->objectAtIndex(i);
					if(target->filename == filename)
					{
						is_ing = true;
					}
				}
				
				if(!is_ing)
				{
					string real_filename = filename;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
					if(real_filename == "sound_casting_attack.mp3" || real_filename == "sound_casting_crash.mp3" ||
					   real_filename == "sound_casting_option.mp3" || real_filename == "sound_bomb_wave.mp3")
					{
						real_filename = real_filename.substr(0,real_filename.length()-3);
						real_filename += "ogg";
					}
#endif
					
					CCLog("PlayEffect loop : %s", filename);
					
					LoopEffect* t_effect = new LoopEffect();
					t_effect->autorelease();
					unsigned int code = mySAE->playEffect(real_filename.c_str(), true);
					t_effect->filename = filename;
					t_effect->effectCode = code;
					loopEffects->addObject(t_effect);
				}
			}
			else
			{
				mySAE->playEffect(filename);
			}
		}
	}
	
	void stopEffect(const char* filename)
	{
		CCLog("StopEffect loop check start : %s", filename);
		bool is_found;
		do
		{
			is_found = false;
			for(int i=0;i<loopEffects->count();i++)
			{
				LoopEffect* target = (LoopEffect*)loopEffects->objectAtIndex(i);
				if(target->filename == filename)
				{
					CCLog("StopEffect loop find : %s", filename);
					is_found = true;
					mySAE->stopEffect(target->effectCode);
					loopEffects->removeObject(target);
					break;
				}
			}
		}while(is_found);
	}
	
	void stopAllEffects()
	{
		CCLog("StopAllEffect loop");
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
	CCArray* loopEffects;
	string playing_sound_name;
	int musicOn;
	bool effectOn;
	
	bool back_down;
	
	AudioEngine() : CCObject()
	{
		mySAE = SimpleAudioEngine::sharedEngine();
		back_down = false;
		
		loopEffects = new CCArray(1);
	}
	
};

#endif
