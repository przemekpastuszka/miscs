function theta = BGD(f, theta, alpha)
  threshold = 10^-8;
  max_iterations = 6;
  errors = zeros(max_iterations);
  [~, gradient] = f(theta);
  iterations = 1;
  while norm(gradient) > threshold && iterations <= max_iterations
      theta = theta - alpha * gradient;
      [error, gradient] = f(theta);
      errors(iterations) = error;
      iterations = iterations + 1;
  end
  
  subplot(2,1,1);
  plot(errors);
  