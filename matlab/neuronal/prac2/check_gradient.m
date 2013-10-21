function val = check_gradient(f, x)
  threshold = 10^-4;
  [~, gradient] = f(x);
  real_gradient = numerical_gradient(f, x);
  absolute = abs(gradient - real_gradient);
  relative = abs(absolute./real_gradient);
  minimas = min([absolute; relative]);
  val = all(minimas < threshold);
       