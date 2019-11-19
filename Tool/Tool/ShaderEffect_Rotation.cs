using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Effects;

namespace Tool
{
    public class ShaderEffect_Rotation : ShaderEffect
    {
        private static PixelShader mPixelShader = new PixelShader() { UriSource = new Uri($"{Environment.CurrentDirectory}\\Resources\\Shaders\\PixelShader_Transform.ps") };

        public ShaderEffect_Rotation()
            : base()
        {
            PixelShader = mPixelShader;

            Rotation = 0.0;
            LeftRightSwap = 0.0;
            TopDownSwap = 0.0;

            UpdateShaderValue(InputProperty);
            UpdateShaderValue(RotationProperty);
            UpdateShaderValue(LeftRightSwapProperty);
            UpdateShaderValue(TopDownSwapProperty);
        }

        public static readonly DependencyProperty InputProperty = ShaderEffect.RegisterPixelShaderSamplerProperty("Input", typeof(ShaderEffect_Rotation), 0);
        public Brush Input
        {
            get { return (Brush)GetValue(InputProperty); }
            set { SetValue(InputProperty, value); }
        }

        public static readonly DependencyProperty DesaturationFactorProperty = DependencyProperty.Register("DesturationFactor", typeof(double), typeof(ShaderEffect_Rotation), new UIPropertyMetadata(0.0, PixelShaderConstantCallback(0), CoerceDesaturationFactor));
        public double DesaturationFactor
        {
            get { return (double)GetValue(DesaturationFactorProperty); }
            set { SetValue(DesaturationFactorProperty, value); }
        }

        public static readonly DependencyProperty RotationProperty = DependencyProperty.Register("Rotation", typeof(double), typeof(ShaderEffect_Rotation), new UIPropertyMetadata(0.0, PixelShaderConstantCallback(0)));
        public double Rotation
        {
            get { return (double)GetValue(RotationProperty); }
            set { SetValue(RotationProperty, value); }
        }

        public static readonly DependencyProperty LeftRightSwapProperty = DependencyProperty.Register("LeftRightSwap", typeof(double), typeof(ShaderEffect_Rotation), new UIPropertyMetadata(0.0, PixelShaderConstantCallback(1)));
        public double LeftRightSwap
        {
            get { return (double)GetValue(LeftRightSwapProperty); }
            set { SetValue(LeftRightSwapProperty, value); }
        }

        public static readonly DependencyProperty TopDownSwapProperty = DependencyProperty.Register("TopDownSwap", typeof(double), typeof(ShaderEffect_Rotation), new UIPropertyMetadata(0.0, PixelShaderConstantCallback(2)));
        public double TopDownSwap
        {
            get { return (double)GetValue(TopDownSwapProperty); }
            set { SetValue(TopDownSwapProperty, value); }
        }

        private static object CoerceDesaturationFactor(DependencyObject d, object value)
        {
            ShaderEffect_Rotation effect = (ShaderEffect_Rotation)d;
            double newFactor = (double)value;

            if (newFactor < 0.0 || newFactor > 1.0)
            {
                return effect.DesaturationFactor;
            }

            return newFactor;
        }
    }
}
