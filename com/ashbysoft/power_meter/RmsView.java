// GUI display of 1-second RMS averaged values from power meter logging
package com.ashbysoft.power_meter;

import java.util.Vector;
import java.util.TimeZone;
import java.util.Calendar;

import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;

import java.awt.Graphics;
import java.awt.Component;
import java.awt.Color;
import java.awt.Font;
import java.awt.geom.AffineTransform;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseEvent;
import java.awt.event.KeyListener;
import java.awt.event.KeyEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import javax.swing.SwingUtilities;
import javax.swing.Timer;
import javax.swing.JFrame;

public class RmsView extends Component
	implements Runnable, MouseListener, MouseMotionListener, KeyListener, ActionListener {
	private JFrame appframe;
	private Font vfont;
	private Calendar cal;
	private File rmslog;
	private String status;
	private String measure;
	private boolean loaded = false;
	private long dates[] = null;
	private float samples[] = null;
	private float min=9999999, max=0;
	private boolean doavg = false;
	private int pmax = -1;
	// drag region start/end in pixels
	private int dstart = -1;
	private int dend = -1;
	// mouse hover
	private Timer htime;
	private int mpos = -1;
	// view start/end in samples
	private int vstart = -1;
	private int vend = -1;

	public static void main(String[] args) throws Exception {
		RmsView me = new RmsView(args);
		SwingUtilities.invokeLater(me);
	}

	RmsView(String[] args) throws Exception {
		String deflog = "rms.log";
		if (args.length>0)
			deflog = args[0];
		this.rmslog = new File(deflog);
		if (!rmslog.exists()) {
			throw new Exception("missing rmslog file: "+deflog);
		}
		this.status = "Loading rmslog file: "+deflog;
		this.htime = new Timer(500, this);
	}

	private float toamps(float r) {
		return r/(float)4.73-(float)0.67;	// stupid Java compiler... constants are always double?
	}

	private float scale() {
		return (float)(vend-vstart)/(float)getWidth();
	}

	private float avg(float[] samples, int off, int len) {
		float sum=0;
		if (len<=0) len=1;
		for (int i=off; i<off+len; i++)
			sum+=(i<samples.length)?samples[i]:0;
		return sum/(float)len;
	}

	private float peak(float[] samples, int off, int len) {
		float pk=0;
		if (len<=0) len=1;
		for(int i=off; i<off+len; i++)
			pk=(samples[i]>pk)?samples[i]:pk;
		return pk;
	}

	private boolean ismidnight(long[] dates, int off, int len) {
		if(len<=0) len=1;
		if(dates[off]/86400000!=dates[off+len]/86400000)
			return true;
		return false;
	}

	public void actionPerformed(ActionEvent e) {
		htime.stop();
		if (loaded) {
			float scl = scale();
			int i=(int)((float)mpos*scl)+vstart;
			measure = ""+samples[i]+"/"+dates[i];
			repaint();
		}
	}

	private String get_utc(long epoch) {
		if(cal==null)
			cal=Calendar.getInstance(TimeZone.getTimeZone("UTC"));
		cal.setTimeInMillis(epoch);
		return ""+cal.get(Calendar.YEAR)+"-"+(cal.get(Calendar.MONTH)+1)+"-"+cal.get(Calendar.DAY_OF_MONTH)+" "+cal.get(Calendar.HOUR_OF_DAY)+":"+cal.get(Calendar.MINUTE);
	}

	public void paint(Graphics g) {
		// one-off rotated font generation
		Font fnt = g.getFont();
		if (vfont==null) {
			AffineTransform tr = new AffineTransform();
			tr.rotate(-Math.PI/2.0);
			vfont = fnt.deriveFont(tr);
		}

		// background
		g.setColor(Color.BLACK);
		g.fillRect(0,0,getWidth(),getHeight());

		// drag region
		if (dstart>=0 && dend>=0) {
			g.setColor(Color.DARK_GRAY);
			if (dend<dstart)
				g.fillRect(dend,0,dstart-dend,getHeight());
			else
				g.fillRect(dstart,0,dend-dstart,getHeight());
		}

		// graticule
		float mag = (float)(getHeight()-20)/max;
		g.setColor(Color.LIGHT_GRAY);
		for(int i=0; i<max; i+=max/10) {
			int y = getHeight()-14-(int)((float)i*mag);
			g.drawLine(0,y,getWidth(),y);
			g.drawString(""+i,10,y-2);
		}

		// more graticule and sample data
		if (loaded) {
			float scl = scale();
			g.setFont(vfont);
			for(int i=0; i<getWidth(); i++) {
				int o = (int)((float)i*scl)+vstart;
				if (ismidnight(dates,o,(int)scl)) {
					g.drawLine(i,0,i,getHeight());
					g.drawString(get_utc(dates[o]),i-5,200);
				}
			}

			int ly = getHeight()-14;
			int oy = ly;
			g.setColor(Color.GREEN);
			for (int i=1; i<getWidth(); i++) {
				int o = (int)((float)i*scl)+vstart;
				int y = doavg? oy-(int)(avg(samples,o,(int)scl)*mag): oy-(int)(peak(samples,o,(int)scl)*mag);
				g.drawLine(i-1,ly,i,y);
				ly = y;
			}
			g.setColor(Color.RED);
			g.setFont(vfont);
			g.drawString(get_utc(dates[vstart]),15,200);
			g.drawString(get_utc(dates[vend-1]),getWidth()-5,200);
			g.setFont(fnt);
			g.setColor(Color.YELLOW);
			g.drawString("average:"+doavg+" max:"+max+"@"+pmax+" scl:"+scl+" view:"+vstart+"/"+vend,10,10);
		}

		// status area
		g.setColor(Color.GRAY);
		g.fillRect(0,getHeight()-13,getWidth(),getHeight());
		g.setColor(Color.BLUE);
		g.drawString(status, 10, getHeight()-1);
		if (measure!=null) {
			g.setColor(Color.YELLOW);
			g.drawString(measure, getWidth()-270, getHeight()-1);
		}
	}

	public void mouseClicked(MouseEvent m) {
	}

	public void mouseEntered(MouseEvent m) {
	}

	public void mouseExited(MouseEvent m) {
	}

	public void mousePressed(MouseEvent m) {
		dstart = m.getX();
		dend = -1;
	}

	public void mouseReleased(MouseEvent m) {
	}

	public void mouseMoved(MouseEvent m) {
		htime.restart();
		mpos=m.getX();
		measure=null;
	}

	public void mouseDragged(MouseEvent m) {
		mouseMoved(m);
		if (dstart>=0) {
			dend = m.getX();
			repaint();
		}
	}

	public void keyPressed(KeyEvent k) {
		if (k.getKeyCode()==KeyEvent.VK_Z && dstart>=0 && dend>=0) {
			// reverse if dragged left
			if (dend<dstart) {
				int t = dend;
				dend = dstart;
				dstart = t;
			}
			// calculate new view into samples
			float scl = scale();
			vend = (int)((float)dend*scl)+vstart;
			vstart = (int)((float)dstart*scl)+vstart;
		} else if(k.getKeyCode()==KeyEvent.VK_N) {
			vstart = 0;
			vend = samples.length;
		} else if(k.getKeyCode()==KeyEvent.VK_Q) {
			appframe.dispose();
		} else if(k.getKeyCode()==KeyEvent.VK_A) {
			doavg=!doavg;
		}
		dstart = -1;
		repaint();
	}

	public void keyReleased(KeyEvent k) {
	}

	public void keyTyped(KeyEvent k) {
	}

	public void run() {
		appframe = new JFrame("RMS Viewer");
		appframe.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		appframe.setSize(1000, 600);
		appframe.add(this);
		appframe.setVisible(true);
		appframe.getContentPane().addMouseListener(this);
		appframe.getContentPane().addMouseMotionListener(this);
		appframe.addKeyListener(this);
		new Thread(new Runnable() {
			public void run() {
				Vector<String> tvec = new Vector<String>();
				try {
					BufferedReader bread = new BufferedReader(new FileReader(rmslog));
					String line;
					while ((line=bread.readLine())!=null) {
						tvec.add(line);
					}
					bread.close();
				} catch (Exception e) {
					status = e.toString();
					return;
				}
				samples = new float[tvec.size()];
				dates = new long[tvec.size()];
				for (int i=0; i<tvec.size(); i++) {
					String line = tvec.get(i);
					int col = line.indexOf(':');
					int dsh = line.indexOf('-');
					try {
						dates[i] = Long.parseLong(line.substring(0,col))*1000;
					} catch (NumberFormatException n) {
						dates[i] = System.currentTimeMillis();
					}
					try {
						if (dsh>0)
							samples[i] = toamps(Float.parseFloat(line.substring(col+2, dsh-1)));
						else
							samples[i] = toamps(Float.parseFloat(line.substring(col+2)));
					} catch (NumberFormatException n) {
						samples[i] = 0;
					}
					min = samples[i]<min? samples[i]: min;
					max = samples[i]>max? samples[pmax=i]: max;
				}
				status = "Loaded";
				vstart = 0;
				vend = samples.length;
				loaded = true;
				repaint();
			}
		}).start();
	}
}
