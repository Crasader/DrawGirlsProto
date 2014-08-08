/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2011      Zynga Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

var CircleSprite = cc.Sprite.extend({
    _degree:0,
    ctor:function () {
        this._super();
    },
    draw:function () {
        cc.drawingUtil.setDrawColor4B(255,255,255,255);

        if (this._degree < 0)
            this._degree = 360;
        cc.drawingUtil.drawCircle(cc.PointZero(), 30, cc.DEGREES_TO_RADIANS(this._degree), 60, true);
    },
    myUpdate:function (dt) {
        this._degree -= 6;
    }
});

var CurveDisposition = {CURVE : 1, RIGHTLINE : 0};
var MathmaticalMissileUnit = cc.Sprite.extend(
{
    
    CatMull:function(P0, P1, P2, P3, t, curve)
    {
        if(curve == CurveDisposition.CURVE)
        {
            var x = this.CatMullFunction(2 * P1.x, (-P0.x + P2.x), (2*P0.x - 5*P1.x + 4*P2.x - P3.x), (-P0.x + 3*P1.x- 3*P2.x + P3.x), t, curve);
            var y = this.CatMullFunction(2 * P1.y, (-P0.y + P2.y), (2*P0.y - 5*P1.y + 4*P2.y - P3.y), (-P0.y + 3*P1.y- 3*P2.y + P3.y), t, curve);
            
            return cc.p(x, y);
        }
        else if(curve == CurveDisposition.RIGHTLINE)
        {
            var ret = cc.pAdd(P1, cc.pMult(cc.pSub(P2, P1), t));
            return ret;
        }

        return cc.p(0, 0);
    },
    CatMullLength:function(P0, P1, P2, P3, curve)
    {
        if(curve == CurveDisposition.CURVE)
        {
            return this.smps(this.IntegralTarget,
                                    P0, P1, P2, P3, 0.0, 1.0, 100);
        }
        else if(curve == CurveDisposition.RIGHTLINE)
        {
            return cc.pLength(cc.pSub(P1, P2));
        }
        
        return -1;
    },
    IntegralTarget:function(thiz, P0, P1, P2, P3, t)
    {
        var dx = thiz.CatMullDerivativeFunction(2 * P1.x, (-P0.x + P2.x), (2*P0.x - 5*P1.x + 4*P2.x - P3.x), (-P0.x + 3*P1.x- 3*P2.x + P3.x), t);
        var dy = thiz.CatMullDerivativeFunction(2 * P1.y, (-P0.y + P2.y), (2*P0.y - 5*P1.y + 4*P2.y - P3.y), (-P0.y + 3*P1.y- 3*P2.y + P3.y), t);
        
        var dx2 = dx * dx;
        var dy2 = dy * dy;
        return Math.sqrt(dx2 + dy2);
    },
    CatMullFunction:function(a, b, c, d, t, curve)
    {
        // a + bt + ct2 + dt3;
        return 0.5 * (a + b*t + c*t*t + d*t*t*t);
    },
    CatMullDerivativeFunction:function(a, b, c, d, t)
    {
        return 0.5 * (b + 2*c*t + 3*d*t*t);
    },
    smps:function(f, A, B, C, D, a, b, n)
    {
        var h = ((b-a)/n);
        var sum = 0;
        for(var i=0; i<n; i++)
        {
            sum += (f(this, A,B,C,D,a+i*h)+4*f(this, A,B,C,D,a+i*h + h/2) + f(this, A,B,C,D,a+(i+1)*h)) * h / 6;
        }
        return sum;
    },

    ctor:function(t_sp, t_angle, t_speed, imgFilename, t_cs, path, curve, fn)
    {
        this._super();
        this.initWithFile(imgFilename);
        this.m_frameCount = 0;
        this.m_isChecking = true;
        this.m_angle = t_angle;
        this.m_speed = t_speed;
        this.m_crashSize = t_cs;
        this.firePosition = t_sp;
        
        this.m_fileName = fn;
        this.m_catmullIndex = 0;
        this.m_catmullvar = 0.0;
        
//      m_catmullPath = {ccp(-50, -50), ccp(0,0), ccp(50, 50), ccp(100, 0), ccp(150, 50),
//          ccp(200, 0), ccp(250, 50), ccp(300, 0), ccp(350, 50), ccp(400, 50),
//          ccp( 450, 0), ccp(500, 50)};
        this.m_catmullPath = path;
        this.m_curve = curve;
        this.m_fileName = fn;
        //      m_catmullPath = {ccp(-25, 0), ccp(0,0), ccp(25, 0), ccp(50, 0),
        //          ccp(75, 0), ccp(100, 0), ccp(125, 0), ccp(150, 0), ccp(175, 0),
        //          ccp(200, 0), ccp(250, 0)};
        this.setPosition(t_sp);

        this.schedule(this.move);
        return true;
    },
    move:function(dt)
    {
        this.setRotation(this.getRotation()-2);
        var beforePosition = cc.pAdd(this.getPosition(), this.getParent().getPosition());
        var r_p = this.getPosition(); // recent
        
        var dv = cc.p(0, 0);
        var len = 0;
        if(this.m_catmullIndex + 4 <= this.m_catmullPath.length)
        {
            dv = this.CatMull(this.m_catmullPath[this.m_catmullIndex], this.m_catmullPath[this.m_catmullIndex+1],
                                     this.m_catmullPath[this.m_catmullIndex+2], this.m_catmullPath[this.m_catmullIndex+3],
                                    Math.min(1.0, this.m_catmullvar), this.m_curve);
            len = this.CatMullLength(this.m_catmullPath[this.m_catmullIndex], this.m_catmullPath[this.m_catmullIndex+1],
                                            this.m_catmullPath[this.m_catmullIndex+2],this. m_catmullPath[this.m_catmullIndex+3],
                                            this.m_curve);
            this.setVisible(true);
        }
        else
             this.setVisible(false);   
        
//      CCLog("dis %f - %d, len = %f, u %f", ccpLength(prevPosition - dv), m_catmullIndex, len, m_catmullvar);
        this.m_catmullvar += this.m_speed / len;
        if(this.m_catmullvar > 1.0)
        {
            if(this.m_catmullIndex + 4 <= this.m_catmullPath.length)
            {
                this.m_catmullvar = 0.0;
                this.m_catmullIndex++;
            }
            else
            {
                this.m_catmullvar = 0.0;
                this.m_catmullIndex = 0;

                // 곡선이 끝났을 때 지우는 코드.
                // this.unschedule(this.move);
                // this.removeFromParentAndCleanup(true);
                // return;
                return;
            }
        }
        var angleRad = this.m_angle * Math.PI / 180;
        dv = cc.p(dv.x * Math.cos(angleRad) - dv.y * Math.sin(angleRad), dv.x * Math.sin(angleRad) + dv.y * Math.cos(angleRad));

//      if(ccpLength(dv) != 0)
//          dv = dv / ccpLength(dv);
//      dv = dv * m_speed;
//      CCLog("Dv = %f %f", dv.x, dv.y);
        r_p = cc.pAdd(this.firePosition, dv);
        // 범위 넘어갔을 때 지우는 코드.
        // var p_p = cc.pAdd(this.getParent().getPosition(), r_p); // parent
        // if(p_p.x < 0.0 - 40.0 || p_p.x > 320.0 + 40.0 || p_p.y < -60.0 - 40.0 || p_p.y > 490.0 + 40.0) // fixed 40.f
        // {
        //     this.unschedule(this.move);
        //     this.removeFromParentAndCleanup(true);
        //     return;
        // }   
   
        this.setPosition(r_p);
        this.m_frameCount++;
    },
    setCatMullPath:function(path)
    {
        var p = JSON.parse(JSON.stringify(path));
        this.m_catmullPath = p;
    }
});


var Helloworld = cc.Layer.extend({
    isMouseDown:false,
    helloImg:null,
    helloLabel:null,
    circle:null,
    sprite:null,
    m_catmullPath:[],
    m_pointSprites:[],
    init:function () {
        var selfPointer = this;
        //////////////////////////////
        // 1. super init first
        this._super();
        /////////////////////////////
        // 2. add a menu item with "X" image, which is clicked to quit the program
        //    you may modify it.
        // ask director the window size
        var size = cc.Director.getInstance().getWinSize();
        
        this.circle = new CircleSprite();
        this.circle.setPosition(cc.p(40, size.height - 60));
        this.addChild(this.circle, 2);
        this.circle.schedule(this.circle.myUpdate, 1 / 60);

        this.setTouchEnabled(true);
        // var jsonCoord = [{x:0, y:0}, {x:0, y:0}, {x:100, y:-50}, {x:150, y:50}, {x:200, y:-50},
        //                                     {x:250, y:50}, {x:300, y:-50}, {x:350, y:50}, {x:400, y:-50}, {x:450, y:0}];

        // for(var i = 0; i<jsonCoord.length; i++)
        // {
        //     jsonCoord[i].y += 160;
        //     jsonCoord[i].x += 240;
        // } 
        var jsonCoord = [];       

        var boss = new cc.Sprite.create("res/test_ui_event.png");
        this.addChild(boss, 0);
        boss.setPosition(cc.p(480, 320));

        this.mmu = new MathmaticalMissileUnit(cc.p(0, 0), 0, 4.0, 
            "res/cumber_missile1.png", cc.SizeMake(4, 4), jsonCoord,
            CurveDisposition.CURVE);
        this.addChild(this.mmu, 10);
        this.mmu.setPosition(cc.p(50, 50));
               
        var thiz = this;
        $("#btnBack").click(function(){
            cc.log(thiz.m_catmullPath.length);
            if(thiz.m_catmullPath.length <= 0 || thiz.m_pointSprites.length <= 0)
                return;

            thiz.m_catmullPath.pop();
            thiz.m_pointSprites.pop().removeFromParent();
            thiz.mmu.setCatMullPath(thiz.m_catmullPath);
            var temp = JSON.parse(JSON.stringify(thiz.m_catmullPath));

            for(var i = 0; i < temp.length; i++)
            {
                temp[i].x -= 480;
                temp[i].y -= 320;
            }
            $("#output").val(JSON.stringify(temp));
        });

        $("#output").change(function(){
            var temp = JSON.parse($("#output").val());
            for(var i=0; i<temp.length; i++)
            {
                temp[i].x += 480;
                temp[i].y += 320;
            }
            for(var i=0; i<thiz.m_pointSprites.length; i++)
            {
                thiz.m_pointSprites[i].removeFromParent();    
            }
            for(var i=0; i<temp.length; i++)
            {
                var pointSprite = cc.Sprite.create("res/cumber_missile9.png");
                pointSprite.setPosition(cc.p(temp[i].x, temp[i].y));
                thiz.addChild(pointSprite, 1);
                thiz.m_pointSprites.push(pointSprite); 
            }
            
            thiz.m_catmullPath = temp;
            thiz.mmu.setCatMullPath(thiz.m_catmullPath);

        });
        $("#curve").click(function(){
            var r = $("#curve").is(':checked');
            cc.log(r);
            if(r)
            {
                thiz.mmu.m_curve = CurveDisposition.CURVE;
                cc.log("check!");

            }
            else
            {
                thiz.mmu.m_curve = CurveDisposition.RIGHTLINE;
                cc.log("not check!");
            }
        });
        return true;
    },
    // a selector callback
    menuCloseCallback:function (sender) {
        cc.Director.getInstance().end();
    },
    onTouchesBegan:function (touches, event) {
        this.isMouseDown = true;
        var xy = touches[0].getLocation();
        var pointSprite = cc.Sprite.create("res/cumber_missile9.png");
        pointSprite.setPosition(cc.p(xy.x, xy.y));
        this.addChild(pointSprite, 1);
        this.m_pointSprites.push(pointSprite);

        this.m_catmullPath.push({x:parseInt(xy.x), y:parseInt(xy.y)});
        this.mmu.setCatMullPath(this.m_catmullPath);
        var temp = JSON.parse(JSON.stringify(this.m_catmullPath));

        for(var i = 0; i < temp.length; i++)
        {
            temp[i].x -= 480;
            temp[i].y -= 320;
        }
        $("#output").val(JSON.stringify(temp));
        // for(var i = 0; i < this.m_catmullPath.length; i++)
        // {

        // }
        
    },
    onTouchesMoved:function (touches, event) {
        if (this.isMouseDown) {
            if (touches) {
                //this.circle.setPosition(cc.p(touches[0].getLocation().x, touches[0].getLocation().y));
            }
        }
    },
    onTouchesEnded:function (touches, event) {
        this.isMouseDown = false;
    },
    onTouchesCancelled:function (touches, event) {
        console.log("onTouchesCancelled");
    }
});

var HelloWorldScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new Helloworld();
        layer.init();
        this.addChild(layer);
    }
});






